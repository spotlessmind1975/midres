<?php

    /*****************************************************************************
     * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
     *****************************************************************************
    * Copyright 2020 Marco Spedaletti (asimov@mclink.it)
    *
    * Licensed under the Apache License, Version 2.0 (the "License");
    * you may not use this file except in compliance with the License.
    * You may obtain a copy of the License at
    *
    * http://www.apache.org/licenses/LICENSE-2.0
    *
    * Unless required by applicable law or agreed to in writing, software
    * distributed under the License is distributed on an "AS IS" BASIS,
    * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    * See the License for the specific language governing permissions and
    * limitations under the License.
    *----------------------------------------------------------------------------
    * Concesso in licenza secondo i termini della Licenza Apache, versione 2.0
    * (la "Licenza"); è proibito usare questo file se non in conformità alla
    * Licenza. Una copia della Licenza è disponibile all'indirizzo:
    *
    * http://www.apache.org/licenses/LICENSE-2.0
    *
    * Se non richiesto dalla legislazione vigente o concordato per iscritto,
    * il software distribuito nei termini della Licenza è distribuito
    * "COSÌ COM'È", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o
    * implicite. Consultare la Licenza per il testo specifico che regola le
    * autorizzazioni e le limitazioni previste dalla medesima.
    ****************************************************************************/

    // IMF TOKENS
    define('IMF_TOKEN_WAIT1', 0xff); // W1 <byte> : wait for <byte> ticks
    define('IMF_TOKEN_WAIT2', 0xfe); // W2 <byte lo> <byte hi> : wait for <word> ticks
    define('IMF_TOKEN_NOTE', 0x40); // NN <byte0> <byte1> : emit note <byte1> with <byte0[2 bits]> amplitude on channel <byte0[hi]>
    define('IMF_TOKEN_CONTROL', 0xd0); // CC <byte0> <byte1> : change control parameter <byte1> change to value <byte2> for channel <byte0>
    define('IMF_TOKEN_PROGRAM_CHANGE', 0xe0); // PP <byte0> <byte1> : change instrument for channel <byte0> to <byte1>
    
    // This autoloader (and uses) are needed to integrate "PHP MIDI Parser"
    // https://github.com/tmont/midiparser
    use \Tmont\Midi\Parsing\FileParser;
	use \Tmont\Midi\Event as Event;
	use \Tmont\Midi\Delta as Delta;
    use \Tmont\Midi\FileHeader as FileHeader;
    use \Tmont\Midi\TrackHeader as TrackHeader;
    use \Tmont\Midi\Event\ProgramChangeEvent as ProgramChangeEvent;
    use \Tmont\Midi\Util\Note as Note;
    spl_autoload_register(function($className)
    {
        $namespace=str_replace("\\","/",__NAMESPACE__);
        $className=str_replace("Tmont/Midi/", '', str_replace("\\","/",$className) );
        $class=__DIR__."/../midiparser/src/"."{$className}.php";
        include_once($class);
    });

    // Translation requires a lot of memory resources, on average more than the 
    // memory available for PHP scripts. If that's not enough, you can increase it. 
    ini_set('memory_limit', '2048M');

    // Auxiliary class, used to "mark" a delay space into the
    // intermediary (not normalized) command stream.
    class PauseMark { }

    // In a music sequencer and MIDI clock, pulses per quarter note (PPQN), also 
    // known as pulses per quarter (PPQ), and ticks per quarter note (TPQN), is 
    // the smallest unit of time used for sequencing note and automation events.
    $ppq = 240;

    // This is the last tempo given from the "SetTempoEvent".
    $tempo = 81;

    // Allowed number of channels
    $allowedChannels = 32768;

    // Remap channels
    $remapChannels = [];

    // Maximum output size (in bytes, 0 = no limit)
    $maximumOutputSize = 0;

    // Input file name (*.mid)
    $inputFileName = null;

    // Output file name (*.imf)
    $ouputFileName = null;

    // Associative array of the channels and notes.
    $channels = [];

    // Isomorphic Music Format tracks
    $imfTracks = [];

    // Actual examined track
    $imfActualTrack = 0;

    // Isomorphic Music Format track
    $imfTrack = [];

    // Transpose controller commands?
    $transposeControllerCommands = false;

    // Transpose program change commands?
    $transposeProgramChangeCommands = false;

    // Forced tempo (ignoring the one into the score)
    $forcedTempo = -1;

    // Count the jiffies needed for delays
    $jiffies = 0;

    // Verbose?
    $verbose = false;

    // Debug?
    $debug = false;

    // This auxiliary function will calculate the equivalence ticks <-> jiffies.
    function calculate_jiffies( $ticks ) {
        global $ppq, $tempo;
        $jiffiesPerTick = ( 300 / ($tempo * $ppq) );
        return (int)(($ticks * $jiffiesPerTick))+1;
    }

    // Show script usage and exit.
    function show_usage_and_exit() {
        echo "--------------------------------------------------------\n";
        echo "midi2hmf - convert MIDI to IMF (Isomorphic Music Format)\n";
        echo "--------------------------------------------------------\n";
        echo " \n";
        echo "Copyright(c) 2020 by Marco Spedaletti.\n";
        echo "Licensed under Apache 2.0 License\n";
        echo " \n";
        echo "Usage: midi2imf.php [options]\n";
        echo "\n";
        echo "options:\n";
        echo "\n";
        echo "[mandatory]\n";
        echo "\n";
        echo " -i <filename> input filename ([ext: *.mid])\n";
        echo "\n";
        echo "[optional]\n";
        echo "\n";
        echo " -C            transpose the 'controller' command(s)\n";
        echo " -c <n>        truncate channels to <n> (ordered by notes)\n";
        echo " -d            additional MIDI debug verbosity\n";
        echo " -f <a> <b>    force channel <a> to be <b> \n";
        echo " -F            force all channels to be 0 (monochannel) \n";
        echo " -o <filename> output filename ([ext: *.imf])\n";
        echo "               By default, like the input with the .imf extension.\n";
        echo " -P            transpose the 'program change' command(s)\n";
        echo " -s <b>        limit output to <b> bytes \n";
        echo " -t <t>        set tempo to <t>\n";
        echo " -v            enable MIDI decode verbosity\n";
        echo "\n";
        exit(0);
    }

    // If there is no option, show help and exit.
    if ( $argc < 1 ) {
        show_usage_and_exit();
    }

    // We decode the various options on the command line.
    for( $i=1; $i<$argc; ++$i ) {

        // Options must start with "-".
        if ( trim($argv[$i][0]) != '-' ) {
            echo "Options must start with '-': '".$argv[$i]."'\n";
            show_usage_and_exit();
        }

        // Decode the option:
        switch( $argv[$i][1] ) {

            // -C            transpose the 'controller' command(s)
            case 'C':
                $transposeControllerCommands = true;
                break;

            // -c <n>        truncate channels to <n> (ordered by notes)
            case 'c':
                ++$i;
                if ( !isset($argv[$i]) || !is_numeric($argv[$i]) ) {
                    echo "Missing number of channels for option -c.\n";
                    show_usage_and_exit();
                }
                $allowedChannels = (int)$argv[$i];
                break;

            // -d            additional MIDI debug verbosity
            case 'd':
                $debug = true;
                break;

            // -f <a> <b>    force channel <a> to be <b>
            case 'f':
                ++$i;
                if ( !isset($argv[$i]) || !is_numeric($argv[$i]) ) {
                    echo "Missing source (<a>) channel for -f.\n";
                    show_usage_and_exit();
                }
                $sourceChannel = (int)$argv[$i];
                ++$i;
                if ( !isset($argv[$i]) || !is_numeric($argv[$i]) ) {
                    echo "Missing target (<b>) channel for -f.\n";
                    show_usage_and_exit();
                }
                $remapChannels[$sourceChannel] = (int)$argv[$i];
                break;

            // -F            force all channels to be 0 (monochannel) 
            case 'F':
                for($c=0; $c<16; ++$c) {
                    $remapChannels[$c] = 0;
                }
                break;

            // -i <filename> input filename ([ext: *.mid])
            case 'i':
                ++$i;
                if ( !isset($argv[$i]) ) {
                    echo "Missing <filename> for option -i.\n";
                    show_usage_and_exit();
                }
                $inputFileName = $argv[$i];
                break;

            // -o <filename> output filename ([ext: *.imf])
            case 'o':
                ++$i;
                if ( !isset($argv[$i]) ) {
                    echo "Missing <filename> for option -o.\n";
                    show_usage_and_exit();
                }
                $outputFileName = $argv[$i];
                break;

            // -P            transpose the 'program change' command(s)
            case 'P':
                $transposeProgramChangeCommands = true;
                break;

            // -s <b>        limit output to <b> bytes 
            case 's':
                ++$i;
                if ( !isset($argv[$i]) || !is_numeric($argv[$i]) ) {
                    echo "Missing bytes (<b>) for option -s.\n";
                    show_usage_and_exit();
                }
                $maximumOutputSize = (int)$argv[$i];
                break;

            // -t <t>        set tempo to <t>
            case 't':
                ++$i;
                if ( !isset($argv[$i]) || !is_numeric($argv[$i]) ) {
                    echo "Missing tempo (<t>) for option -t.\n";
                    show_usage_and_exit();
                }
                $forcedTempo = (int)$argv[$i];
                $tempo = $forcedTempo;
                break;

            // -v            enable MIDI decode verbosity
            case 'v':
                $verbose = true;
                break;

            // By default, unknown options is an error.
            default:
                echo "Unknown option '".$argv[$i]."'\n";
                show_usage_and_exit();
        }
    }

    // Exit if the input file name is missing.
    if ( is_null($inputFileName) ) {
        echo "Missing input filename's option (-i).\n";
        show_usage_and_exit();
    }

    // If the output file name is missing, the input file name
    // has the same name of the input file name but different
    // extension (.mid -> .imf).
    if ( is_null($outputFilename) ) {
        $outputFileName = preg_replace("#.mid$#", ".imf", $inputFileName);
    }

    /*************************************************************************
     *** STAGE ZERO: PRE-ANALISYS OF THE MIDI FILE
     *************************************************************************/

    try {

        $parser = new FileParser();
        $parser->load($inputFileName);

        // We go through the whole file, to extract the (indirect) information
        // that are needed, as the number of tracks, PPQ, notes and channels.
        while ($chunk = $parser->parse()) {

            if ($chunk instanceof FileHeader ) {

                list($midiFormat, $numTracks, $ppq) = $chunk->getData();

            } else if ( $chunk instanceof Event\NoteOnEvent ) {

                // We build an associative array, which allows to obtain a double information: 
                // the number (and cardinality) of the channels as well as the number of notes
                // for each channel.
                list($channel) = $chunk->getData();
                if ( !isset( $channels[$channel] ) ) {
                    $channels[$channel] = 0;
                }
                $channels[$channel]++;

            }

        }

    } catch( \Exception $e ) {
        echo "ERROR DURING STAGE ZERO: ". $e->getMessage()."\n";
        show_usage_and_exit($_argv);   
    }

    // We sort the array by the number of notes, so that the channels with the 
    // largest number of notes are on top.
    arsort($channels);

    // We extract only the index of the channels that actually want to convert.
    $effectiveChannels = [];
    foreach($channels as $channel => $c ) {
        $effectiveChannels[] = (int)$channel;
        if ( count($effectiveChannels) == $allowedChannels ) {
            break;
        }
    }

    // Show the summary if the verbosity is enough.
    if ( $verbose ) {
        echo "+------------------------------------------------------+\n";
        echo "|             MIDI FILE SPECIFICATIONS                 |\n";
        echo "+------------------------------------------------------+\n";
        echo "| FORMAT  : ".str_pad($midiFormat, 42, " ", STR_PAD_RIGHT)." |\n";
        echo "| TRACKS  : ".str_pad($numTracks, 42, " ", STR_PAD_RIGHT)." |\n";
        echo "| TEMPO   : ".str_pad($tempo." (1 tick = ".calculate_jiffies(1)." jiffies)", 42, " ", STR_PAD_RIGHT)." |\n";
        echo "| CHANNELS: ".str_pad((count($channels))." (effective: ".count($effectiveChannels).")", 42, " ", STR_PAD_RIGHT)." |\n";
        foreach( $effectiveChannels as $channel ) {
            echo "|       #".str_pad($channel, 2, "0",STR_PAD_LEFT);
            echo " ";
            if ( isset($remapChannels[$channel]) ) {
                echo str_pad(" > #".str_pad($remapChannels[$channel], 2, "0",STR_PAD_LEFT), 7, " ",STR_PAD_LEFT);
            } else {
                echo str_pad(" ", 7, " ",STR_PAD_LEFT);
            }
            echo ": ".str_pad($channels[$channel]." notes", 33, " ", STR_PAD_RIGHT)." |\n";
        }
        echo "+------------------------------------------------------+\n";
    } else {
        echo "+------------------------------------------------------+\n";
    }

    echo ": ".str_pad("MIDI TRACK DECODING", 52, " ", STR_PAD_BOTH)." |\n";
    echo "+------------------------------------------------------+\n";

    /*************************************************************************
     *** STAGE ONE: MIDI TRACK DECODING
     *************************************************************************/

    try {

        $parser = new FileParser();
        $parser->load($inputFileName);

        // We go through the whole file, to extract the notes, delays and so on.
        while ($chunk = $parser->parse()) {

            // We intercept the beginning of each track, in order to map it 
            // with an IMF track.
            if ($chunk instanceof TrackHeader) {

                // We add the track only if we have inserted at least 
                // one note in the track.
                if ( count($imfTrack) > 0 ) {
                    $imfTracks[] = $imfTrack;
                    $imfTrack = [];
                }

                //echo "\n| TR | ";

            // We intercept the time change to take it into account 
            // in terms of delay(s).
            } else if ($chunk instanceof Event\SetTempoEvent && $forcedTempo == -1 ) {

                list($a, $b, $c) = $chunk->getData();

                $mpqn = ($a << 16) | ($b << 8) | $c;
                $tempo = Event\SetTempoEvent::getBpmFromMpqn($mpqn);

                if ( $verbose && $debug ) {
                    echo "+-----------------------\n";
                    echo "| NEW TEMPO: ".$tempo." (1 tick = ".calculate_jiffies(1)." jiffies)\n";
                    echo "+-----------------------\n";
                } else if ( $verbose ) {
                    // echo "T";
                }

            // We intercept the starting of a note.
            } else if ($chunk instanceof Event\NoteOnEvent) {

                list($channel, $noteNumber, $velocity) = $chunk->getData();

                // We include notes only for those channels 
                // that are actually required or permitted.
                if ( in_array($channel, $effectiveChannels ) ) {

                    $imfTrack[] = $chunk;
    
                    if ( $verbose && $debug ) {
                        echo "| ON  [".str_pad($channel,STR_PAD_RIGHT, " ", 2)."] [".str_pad($noteNumber,STR_PAD_RIGHT, " ", 3)."] ".str_pad(Note::getNoteName($noteNumber),STR_PAD_LEFT, " ", 6)." at ".$velocity."\n";
                    } else if ( $verbose ) {
                        //echo "N";
                    }
                } else {
                    //echo "x";
                }

            // We intercept any delay.
            } else if ($chunk instanceof Delta) {

                list($ticks) = $chunk->getData();
                if ( $ticks == 0 ) continue;

                // We transform the delays into as many wait commands, 
                // proportional to the actual duration of the delay.
                $jiffies = calculate_jiffies( $ticks );
                $originalJiffies = $jiffies;

                while($jiffies) {
                    $imfTrack[] = new PauseMark;
                    --$jiffies;
                }

                if ( $verbose && $debug ) {
                    echo "+-----------------------\n";
                    echo "| ".$ticks." (".$originalJiffies." jiffies)\n";
                    echo "+-----------------------\n";
                } else if ( $verbose ) {
                    //echo "D (".dechex($ticks).")";
                }

            // We intercept the ending of a note.
            } else if ($chunk instanceof Event\NoteOffEvent) {

                list($channel, $noteNumber, $velocity) = $chunk->getData();

                // We include notes only for those channels 
                // that are actually required or permitted.
                if ( in_array($channel, $effectiveChannels ) ) {
                    $imfTrack[] = $chunk;
                    if ( $verbose && $debug ) {
                        echo "| OFF [".str_pad($channel,STR_PAD_RIGHT, " ", 2)."] [".str_pad($noteNumber,STR_PAD_RIGHT, " ", 3)."] ".str_pad(Note::getNoteName($noteNumber),STR_PAD_LEFT, " ", 6)." at ".$velocity."\n";
                    } else if ( $verbose ) {
                        //echo "O";
                    }
                } else {
                    //echo "x";
                }

            // We intercept any controlling event.
            } else if ($chunk instanceof Event\ControllerEvent && $transposeControllerCommands) {

                list($channel, $parameterNumber, $value) = $chunk->getData();

                // We include events only for those channels 
                // that are actually required or permitted.
                if ( in_array($channel, $effectiveChannels ) ) {

                    $imfTrack[] = $chunk;
    
                    if ( $verbose && $debug ) {
                        echo "| PAR [".str_pad($channel,STR_PAD_RIGHT, " ", 2)."] [".str_pad($parameterNumber,STR_PAD_RIGHT, " ", 3)."] = ".$value."\n";
                    } else if ( $verbose ) {
                        //echo "P";
                    }
                } else {
                    //echo "x";
                }

            // We intercept any program change event.
            } else if ($chunk instanceof ProgramChangeEvent && $transposeProgramChangeCommands) {

                list($channel, $instrument) = $chunk->getData();

                // We include events only for those channels 
                // that are actually required or permitted.
                if ( in_array($channel, $effectiveChannels ) ) {

                    $imfTrack[] = $chunk;

                    if ( $verbose && $debug ) {
                        echo "+ CHG [".str_pad($channel,STR_PAD_RIGHT, " ", 2)."] [".str_pad($instrument,STR_PAD_RIGHT, " ", 3)."]\n";
                    } else if ( $verbose ) {
                        //echo "C";
                    }
                } else {
                    //echo "x";
                }

            } else {
                if ( $verbose && $debug ) {
                    //echo "unknown: ".get_class($chunk)."\n";
                } else {
                    //echo "?";
                }
            }

        }

        // Let's add the last useful track.
        if ( count($imfTrack) > 0 ) {
            $imfTracks[] = $imfTrack;
            $imfTrack = [];
        }

        // We count the decoded commands.
        $maxCommands = 0;
        for( $i=0; $i<count($imfTracks); ++$i ) {
            if ( $maxCommands < count($imfTracks[$i]) ) {
                $maxCommands = count($imfTracks[$i]);
            }
        }

        // Show the summary if the verbosity is enough.
        if ( $verbose ) {
            echo "| TRACKS  : ".str_pad(count($imfTracks), 42, " ", STR_PAD_RIGHT)." |\n";
            for( $i=0; $i<count($imfTracks); ++$i ) {
                echo "|       #".str_pad($i, 2, "0",STR_PAD_LEFT);
                echo " ";
                echo ": ".str_pad(count($imfTracks[$i])." commands", 40, " ", STR_PAD_RIGHT)." |\n";
            }
            echo "+------------------------------------------------------+\n";
        } else {
            echo "+------------------------------------------------------+\n";
        }

    } catch( \Exception $e ) {
        echo "ERROR DURING STAGE ONE: ". $e->getMessage()."\n";
        show_usage_and_exit($_argv);   
    }

    /*************************************************************************
     *** STAGE TWO: TRACKS INTERPOLATION
     *************************************************************************/

    echo ": ".str_pad("TRACKS INTERPOLATION", 52, " ", STR_PAD_BOTH)." |\n";
    echo "+------------------------------------------------------+\n";

    try {

        $imfData = [];

        // Let's go through all the commands on each track, 
        // using the longest track as a guide.
        for($command=0; $command<$maxCommands; ++$command) {

            // We check for a pause for each track.
            for($track=0; $track<count($imfTracks); ++$track) {
                $isPause = true;
                if ( isset($imfTracks[$track][$command]) && !($imfTracks[$track][$command] instanceof PauseMark) ) {
                    $isPause = false;
                    break;
                }
            }

            // If there is a pause in all tracks, we record a pause.
            if ( $isPause ) {
                $imfData[] = new PauseMark;
            } else {
                // We transcribe the commands (except the pauses).
                for($track=0; $track<count($imfTracks); ++$track) {
                    if ( isset($imfTracks[$track][$command]) && !($imfTracks[$track][$command] instanceof PauseMark) ) {
                        $imfData[] = $imfTracks[$track][$command];
                    }
                }
            }
        }

    } catch( \Exception $e ) {
        echo "ERROR DURING STAGE TWO: ". $e->getMessage()."\n";
        show_usage_and_exit($_argv);   
    }

    echo ": ".str_pad("WRITING IMF STREAM", 52, " ", STR_PAD_BOTH)." |\n";
    echo "+------------------------------------------------------+\n";

    $fh = fopen($outputFileName, "wb");

    // We write the consolidated data.
    foreach( $imfData as $chunk ) {

        // We intercept the pause commands, 
        // to calculate the length of the delays.
        if ($chunk instanceOf PauseMark ) {

            ++$jiffies;

        } else if ($chunk instanceof Event\NoteOnEvent) {

            // Every time we issue a note, we need to check if there have been
            // any delay(s). If so, we need to bring them back to the outgoing
            // stream.
            if ( $jiffies ) {

                // To optimize the output stream, we only use one byte if the
                // delay is less than 255 ticks.
                if ( $jiffies < 255 ) {
                    fputs($fh, chr(IMF_TOKEN_WAIT1), 1 );
                    fputs($fh, chr($jiffies), 1 );
                    echo "w";
                } else {
                    fputs($fh, chr(IMF_TOKEN_WAIT2), 1 );
                    fputs($fh, chr($jiffies & 0xff), 1 );
                    fputs($fh, chr(( $jiffies >> 8 )), 1 );
                    echo "W (".dechex($jiffies).") ";
                }

                // Reset the jiffies' count.
                $jiffies = 0;
            }

            list($channel, $noteNumber, $velocity) = $chunk->getData();

            // We replace the channel if necessary.
            if ( isset($remapChannels[$channel]) ) {
                $channel = $remapChannels[$channel];
            }
            fputs($fh, chr($channel | ( ( $velocity >> 6) << 4 ) | IMF_TOKEN_NOTE), 1 );
            fputs($fh, chr($noteNumber), 1 );

            echo "N";

        } else if ($chunk instanceof Event\NoteOffEvent) {

            // Every time we issue a note, we need to check if there have been
            // any delay(s). If so, we need to bring them back to the outgoing
            // stream.
            if ( $jiffies ) {

                // To optimize the output stream, we only use one byte if the
                // delay is less than 255 ticks.
                if ( $jiffies < 255 ) {
                    fputs($fh, chr(IMF_TOKEN_WAIT1), 1 );
                    fputs($fh, chr($jiffies), 1 );
                    echo "w";
                } else {
                    fputs($fh, chr(IMF_TOKEN_WAIT2), 1 );
                    fputs($fh, chr($jiffies & 0xff), 1 );
                    fputs($fh, chr(( $jiffies >> 8 )), 1 );
                    echo "W (".dechex($jiffies).") ";
                }

                // Reset the jiffies' count.
                $jiffies = 0;
            }

            list($channel, $noteNumber, $velocity) = $chunk->getData();

            // We replace the channel if necessary.
            if ( isset($remapChannels[$channel]) ) {
                $channel = $remapChannels[$channel];
            }
            fputs($fh, chr($channel), 1 );

            echo "O";

        } else if ($chunk instanceof Event\ControllerEvent) {

            list($channel, $parameterNumber, $value) = $chunk->getData();

            // We replace the channel if necessary.
            if ( isset($remapChannels[$channel]) ) {
                $channel = $remapChannels[$channel];
            }
            fputs($fh, chr($channel | IMF_TOKEN_CONTROL), 1);
            fputs($fh, chr($parameterNumber), 1);
            fputs($fh, chr($value), 1);
            
            echo "C";

        } else if ($chunk instanceof ProgramChangeEvent) {

            list($channel, $instrument) = $chunk->getData();

            // We replace the channel if necessary.
            if ( isset($remapChannels[$channel]) ) {
                $channel = $remapChannels[$channel];
            }

            fputs($fh, chr($channel | IMF_TOKEN_PROGRAM_CHANGE), 1);
            fputs($fh, chr($instrument), 1);
            
            echo "P";

        }

        // We limit the size of the output file to this value,
        // if assigned by options.
        if ( $maximumOutputSize > 0 ) {
            if ( ftell($fh) >= $maximumOutputSize ) {
                break;
            }
        }

    }

    fclose($fh);

?>