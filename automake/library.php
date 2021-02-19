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

    define('ATR_DENSITY_SINGLE', 0);
    define('ATR_DENSITY_EXTENDED', 1);
    define('ATR_DENSITY_DOUBLE', 2);
    define('ATR_DENSITY_QUAD', 3);

    /////////////////////////////////////////////////////////////////////////////////

function extract_symbols_from_sources( ) {

    $symbols = [];
    $aliases = [];
    $files = glob('src/*.h');

    foreach ( $files as $file ) {

        $content = file($file);

        foreach ( $content as $line ) {

            if ( preg_match('/\s*#define\s*(mr\_[a-zA-Z0-9\_]+).*(_mr\_[a-zA-Z0-9\_]+)/', $line, $matched) ) {
                $aliases[$matched[1]][] = $matched[2];
            } else if ( preg_match('/\s*#define\s*(mr\_[a-zA-Z0-9\_]+).*(mr\_[a-zA-Z0-9\_]+)/', $line, $matched) ) {
                $aliases[$matched[1]][] = $matched[2];
            }

        }

    }

    $files = glob('src/*.c');

    foreach ( $files as $file ) {

        $content = file($file);

        $symbols[$file]['export'] = [];
        $symbols[$file]['import'] = [];

        foreach ( $content as $line ) {

            if ( preg_match('#^\s*[a-z\_]+ [a-z\_\*]+ [\*]?(\_mr\_[A-Za-z0-9\_]+).*{\s*#', $line, $matched) ) {
                $symbols[$file]['export'][$matched[1]] = $matched[1];
            } else if ( preg_match('#^\s*[a-z\_]+ [a-z\_\*]+ [\*]?(mr\_[A-Za-z0-9\_]+).*{\s*#', $line, $matched) ) {
                $symbols[$file]['export'][$matched[1]] = $matched[1];
            } else if ( preg_match('#^\s*[a-z\_\*]+ [\*]?(\_mr\_[A-Za-z0-9\_]+).*{\s*#', $line, $matched) ) {
                $symbols[$file]['export'][$matched[1]] = $matched[1];
            } else if ( preg_match('#^\s*[a-z\_\*]+ [\*]?(mr\_[A-Za-z0-9\_]+).*{\s*#', $line, $matched) ) {
                $symbols[$file]['export'][$matched[1]] = $matched[1];
            } else if ( preg_match('#MR_PT_THREAD_EXT\((mr\_[A-Za-z0-9\_]+)#', $line, $matched ) ) {
                $symbols[$file]['export'][$matched[1]] = $matched[1];
            }

        }

        foreach ( $content as $line ) {

            if ( preg_match('#(_mr\_[A-Za-z0-9\_]+)\s*\(#', $line, $matched) ) {
                if ( !isset( $symbols[$file]['export'][$matched[1]] )) {
                    $symbols[$file]['import'][$matched[1]] = $matched[1];
                    if ( isset($aliases[$matched[1]]) ) {
                        foreach( $aliases[$matched[1]] as $alias ) {
                            $symbols[$file]['import'][$alias] = $alias;
                        }
                    }
                }
            } else if ( preg_match('#(mr\_[A-Za-z0-9\_]+)\s*\(#', $line, $matched) ) {
                if ( !isset( $symbols[$file]['export'][$matched[1]] )) {
                    $symbols[$file]['import'][$matched[1]] = $matched[1];
                    if ( isset($aliases[$matched[1]]) ) {
                        foreach( $aliases[$matched[1]] as $alias ) {
                            $symbols[$file]['import'][$alias] = $alias;
                        }
                    }
                }
            }

        }

        foreach ( $symbols[$file]['import'] as $alias => $symbol ) {
            if ( substr($symbol, strlen($symbol)-3, 3) == '_hd' ) {
                $newSymbol = str_replace('_hd', '_memory_mapped', $symbol);
                $symbols[$file]['import'][$newSymbol] = $newSymbol;
            }
            if ( substr($symbol, strlen($symbol)-5, 5) == '_file' ) {
                $newSymbol = str_replace('_file', '_file_memory_mapped', $symbol);
                $symbols[$file]['import'][$newSymbol] = $newSymbol;
            }
        }
    
    }

    // foreach( $symbols as $module => $subsymbol ) {
    //     print "# ".$module.":"."\n";
    //     foreach( $subsymbol['export'] as $export ) {
    //         print "#   export: ".$export.":"."\n";
    //     }
    //     foreach( $subsymbol['import'] as $import ) {
    //         print "#   import: ".$import.":"."\n";
    //     }
    // }
    return $symbols;

}

//////////////////////////////////////////////////////////////////////////////////

function build_list_of_needed_files( $symbols, $module, & $neededFiles = [] /*, $dump = false*/ ) {

    // print "# build_list_of_needed_files( [], ".$module.", [], false )\n";

    // if ( $module == 'src/utility_musician.c' ) {
    //     $dump = true;        
    // }

    // if ( $dump ) {
    //     print "# ".$module."\n";
    // }

    if ( isset( $symbols[$module] ) ) {

        // if ( $dump ) {
        //     print "#  [found]\n";
        // }

        foreach( $symbols[$module]['import'] as $import ) {
            
            // if ( $dump ) {
            //     print "#  looking for ".$import."\n";
            // }
    
            foreach( $symbols as $module => $dependences ) {
                // if ( $dump ) {
                //     print "#  >> ".$module." -> [..]\n";
                // }
                if ( isset($neededFiles[$module]) ) {
                    // if ( $dump ) {
                    //     print "#  >> (skipped)\n";
                    // }
                    continue;
                }
                if ( isset( $dependences['export'][$import] ) ) {
                    // if ( $dump ) {
                    //     print "#  >> found!\n";
                    // }
                    $neededFiles[$module] = $module;
                    build_list_of_needed_files( $symbols, $module, $neededFiles, $dump );
                }
            }
        }

    }

}

//////////////////////////////////////////////////////////////////////////////////

function find_main_module( $application_name ) {

    // print "# find_main_module(".$application_name.")\n";

    $files = glob('src/*.c');

    foreach ( $files as $file ) {

        $content = implode('', file($file));

        if ( 
                ( stripos( $content, '#ifdef __'.str_replace('_','',$application_name).'__' ) !== false 
                ||
                stripos( $content, 'defined(__'.str_replace('_','',$application_name).'__)' ) !== false )
            && $file != 'src/main.c') {

            // print "# -> ".$file."\n";
            return $file;

        }

    }

    return null;

}
    
//////////////////////////////////////////////////////////////////////////////////

function is_commodore($platform) {
    $cbm = false;
    switch( $platform ) {
        case 'c64':
        case 'vic20':
        case 'vic2024':
        case 'c16':
        case 'plus4':
        case 'c128':
            $cbm = true;
            break;
    }
    return $cbm;
}

//////////////////////////////////////////////////////////////////////////////////

function get_platform88($platform) {
    $platform88 = $platform;
    switch( $platform ) {
        case 'msxc':
            $platform88 = 'msx';
            break;
        case 'mtx500':
            $platform88 = 'mtx';
            break;
        break;
    }
    return $platform88;
}

//////////////////////////////////////////////////////////////////////////////////

function get_platform65($platform) {
    $platform65 = $platform;
    switch( $platform ) {
        case 'vic2024':
            $platform65 = 'vic20';
            break;
        case 'atarilo':
            $platform65 = 'atari';
            break;
    }
    return $platform65;
}

//////////////////////////////////////////////////////////////////////////////////

function emit_rules_for_library_cc65($platform) {

    $cbm = is_commodore($platform);

    $platform65 = get_platform65($platform);

    $options = '';
    if ( $platform == 'vic2024') {
        $options = '-D__24K__';
    }

?>

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR <?=strtoupper($platform);?> 
# -------------------------------------------------------------------
midres.embedded.<?=$platform;?>:

obj/<?=$platform;?>/midres_atari_impl.o:	src/midres_atari_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$platform;?>/midres_atari_impl.o src/midres_atari_impl.asm

obj/<?=$platform;?>/midres_c64_impl.o:	src/midres_c64_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$platform;?>/midres_c64_impl.o src/midres_c64_impl.asm

obj/<?=$platform;?>/midres_plus4_impl.o:	src/midres_plus4_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$platform;?>/midres_plus4_impl.o src/midres_plus4_impl.asm

obj/<?=$platform;?>/midres_sid_impl.o:	src/midres_sid_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$platform;?>/midres_sid_impl.o src/midres_sid_impl.asm

obj/<?=$platform;?>/midres_ted_impl.o:	src/midres_ted_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$platform;?>/midres_ted_impl.o src/midres_ted_impl.asm

obj/<?=$platform;?>/midres_pokey_impl.o:	src/midres_pokey_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$platform;?>/midres_pokey_impl.o src/midres_pokey_impl.asm

obj/<?=$platform;?>/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl <?=$cbm?'-D__CBM__':'';?>  -o $@ $(subst obj/<?=$platform;?>/,src/,$(@:.o=.c))

$(LIBDIR)/midres.<?=$platform;?>.lib:	midres.embedded.<?=$platform;?> $(LIB_INCLUDES) $(subst PLATFORM,<?=$platform;?>,$(LIB_OBJS)) obj/<?=$platform;?>/midres_atari_impl.o  obj/<?=$platform;?>/midres_c64_impl.o obj/<?=$platform;?>/midres_plus4_impl.o obj/<?=$platform;?>/midres_sid_impl.o obj/<?=$platform;?>/midres_ted_impl.o obj/<?=$platform;?>/midres_pokey_impl.o
	$(AR) r $(LIBDIR)/midres.<?=$platform;?>.lib $(subst PLATFORM,<?=$platform;?>,$(LIB_OBJS)) obj/<?=$platform;?>/midres_atari_impl.o obj/<?=$platform;?>/midres_c64_impl.o obj/<?=$platform;?>/midres_plus4_impl.o obj/<?=$platform;?>/midres_sid_impl.o obj/<?=$platform;?>/midres_ted_impl.o obj/<?=$platform;?>/midres_pokey_impl.o

<?php
}

//////////////////////////////////////////////////////////////////////////////////

function emit_rules_for_library_z88dk($platform) {

?>

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR <?=strtoupper($platform);?> 
# -------------------------------------------------------------------

lib/midres.<?=$platform;?>.lib:
<?php
}

//////////////////////////////////////////////////////////////////////////////////

function emit_commands_for_create_1541_disk($platform,$filename,$files = []) {

    $diskImage = $filename.'.'.$platform.'.d64';

?>
	$(call RMFILES,$(EXEDIR)/<?=$diskImage;?>)
<?php
    foreach( $files as $file ) {
        $destination = $file['destination'];
        $source = $file['source'];
?>	$(CC1541) -f <?=basename($destination);?> -w <?=$source;?> $(EXEDIR)/<?=$diskImage;?> 
<?php        
    }

}

//////////////////////////////////////////////////////////////////////////////////

function emit_commands_for_create_1571_disk($platform,$filename,$files = []) {

    $diskImage = $filename.'.'.$platform.'.d71';

?>
	$(call RMFILES,$(EXEDIR)/<?=$diskImage;?>)
<?php
    foreach( $files as $file ) {
        $destination = $file['destination'];
        $source = $file['source'];
?>	$(CC1541) -f <?=basename($destination);?> -w <?=$source;?> $(EXEDIR)/<?=$diskImage;?> 
<?php        
    }

}

function emit_commands_for_create_1581_disk($platform,$filename,$files = []) {

    $diskImage = $filename.'.'.$platform.'.d81';

?>
	$(call RMFILES,$(EXEDIR)/<?=$diskImage;?>)
<?php
    foreach( $files as $file ) {
        $destination = $file['destination'];
        $source = $file['source'];
?>	$(CC1541) -f <?=basename($destination);?> -w <?=$source;?> $(EXEDIR)/<?=$diskImage;?> 
<?php        
    }

}

//////////////////////////////////////////////////////////////////////////////////

function emit_commands_for_create_atr_disk($platform,$filename,$files = [], $density = ATR_DENSITY_SINGLE) {

    $extension = '.atr';

    switch( $density ) {
        default:
        case ATR_DENSITY_SINGLE:
            $densityFlag='-S';
            $extension = 'atr';
            break;
        case ATR_DENSITY_EXTENDED:
            $densityFlag='-E';
            $extension = 'ed.atr';
            break;
        case ATR_DENSITY_DOUBLE:
            $densityFlag='-D';
            $extension = 'dd.atr';
            break;
        case ATR_DENSITY_QUAD:
            $densityFlag='-Q';
            $extension = 'qd.atr';
            break;
    }

    $diskImage = $filename.'.'.$platform.'.'.$extension;

    $first = current($files);
    
?>
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
<?php
    foreach( $files as $file ) {
        $destination = $file['destination'];
        $source = $file['source'];
?>	$(call COPYFILES,<?=$source;?>,$(EXEDIR)/atr/<?=str_replace('$(EXEDIR)/','',$destination);?>)
<?php        
    }
?>
	$(DIR2ATR) <?=$densityFlag;?> -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/<?=$diskImage;?> $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/<?=$diskImage;?> -o $(EXEDIR)/<?=$diskImage;?> -f <?=str_replace('$(EXEDIR)/','',$first['destination']);?>

<?php

}

//////////////////////////////////////////////////////////////////////////////////

function emit_rules_for_ancillary_cc65($platform, $demo, $resources = [], $embedded = false ) {

    $cbm = is_commodore($platform);

    $platform65 = get_platform65($platform);

    $options = '';
    if ( $platform == 'vic2024') {
        $options = '-D__24K__';
    }

?>
# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR <?=strtoupper($platform);?> 
# -------------------------------------------------------------------

.PHONY: midres.<?=$demo;?>.embedded.<?=$platform;?> midres.<?=$demo;?>.<?=$platform;?>

midres.<?=$demo;?>.embedded.<?=$platform;?>:
	$(FILE2INCLUDE) <?php
    foreach( $resources as $key => $resource ) {
        if ( isset($resource['loader'] ) ) continue;
        if ( !isset($resource['source']) ) continue;
        $source = $resource['source'];
        $destination = $resource['destination'];
?>-i <?=$source;?> -n <?=$destination;?> <?php 
    }
    ?>-c src/rawdata.c -h src/rawdata.h
<?php
    if ( $embedded ) {
?>
	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl <?=$cbm?'-D__CBM__':'';?> -o obj/<?=$platform65;?>/rawdata.o src/rawdata.c
<?php
    }
?>

<?php
    if ( $embedded ) {
?>
	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl <?=$cbm?'-D__CBM__':'';?> -o obj/<?=$platform65;?>/rawdata.o src/rawdata.c
<?php
    }
?>

obj/<?=$platform;?>/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl <?=$cbm?'-D__CBM__':'';?> -o $@ $(subst obj/<?=$platform;?>/,src/,$(@:.o=.c))

$(EXEDIR)/midres.<?=$demo;?>.<?=$platform;?>: midres.<?=$demo;?>.embedded.<?=$platform;?> $(subst PLATFORM,<?=$platform;?>,$(OBJS))
	$(CC) -Ln demo<?=$platform;?>.lbl -t <?=$platform65;?> -C cfg/<?=$platform;?>.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.<?=$platform;?>.map -o $(EXEDIR)/midres.<?=$platform;?> $(subst PLATFORM,<?=$platform;?>,$(OBJS)) $(LIBDIR)/midres.<?=$platform;?>.lib
<?php 

    switch( $platform ) {
        case 'c64':
        case 'vic20':
        case 'c16':
        case 'c128':
            $executable = [
                "destination" => "midres-single",
                "source" => "$(EXEDIR)/midres.".$platform65,
            ];

            if ( $embedded ) {
                emit_commands_for_create_1541_disk($platform, "midres", [ $executable ]);
                emit_commands_for_create_1571_disk($platform, "midres", [ $executable ]);
                emit_commands_for_create_1581_disk($platform, "midres", [ $executable ]);
            } else {
                emit_commands_for_create_1541_disk($platform, "midres", array_merge([ $executable ], $resources));
                emit_commands_for_create_1571_disk($platform, "midres", array_merge([ $executable ], $resources));
                emit_commands_for_create_1581_disk($platform, "midres", array_merge([ $executable ], $resources));
            }
            break;
        case 'vic2024':
        case 'plus4':
            $loader = array_shift($resources);
            $loader = [
                "destination" => "loader",
                "source" => "$(DATADIR)/".$loader['loader']
            ];
            $executable = [
                "destination" => "midres-single",
                "source" => "$(EXEDIR)/midres.".$platform,
            ];

            if ( $embedded ) {
                emit_commands_for_create_1541_disk($platform, "midres", [ $loader, $executable ]);
                emit_commands_for_create_1571_disk($platform, "midres", [ $loader, $executable ]);
                emit_commands_for_create_1581_disk($platform, "midres", [ $loader, $executable ]);
            } else {
                emit_commands_for_create_1541_disk($platform, "midres", array_merge([ $loader, $executable ], $resources));
                emit_commands_for_create_1571_disk($platform, "midres", array_merge([ $loader, $executable ], $resources));
                emit_commands_for_create_1581_disk($platform, "midres", array_merge([ $loader, $executable ], $resources));
            }
            break;
            break;
        case 'atari':
        case 'atarilo':
            $executable = [
                "destination" => "midres.exe",
                "source" => "$(EXEDIR)/midres.".$platform
            ];
            if ( $embedded ) {
                emit_commands_for_create_atr_disk($platform65, "midres", [ $executable ], ATR_DENSITY_SINGLE);
                emit_commands_for_create_atr_disk($platform65, "midres", [ $executable ], ATR_DENSITY_EXTENDED);
                // emit_commands_for_create_atr_disk($platform65, "midres", [ $executable ], ATR_DENSITY_DOUBLE);
                // emit_commands_for_create_atr_disk($platform65, "midres", [ $executable ], ATR_DENSITY_QUAD);
            } else {
                emit_commands_for_create_atr_disk($platform65, "midres", array_merge( [ $executable ], $resources), ATR_DENSITY_SINGLE);
                emit_commands_for_create_atr_disk($platform65, "midres", array_merge( [ $executable ], $resources), ATR_DENSITY_EXTENDED);
                // emit_commands_for_create_atr_disk($platform65, "midres", array_merge( [ $executable ], $resources), ATR_DENSITY_DOUBLE);
                // emit_commands_for_create_atr_disk($platform65, "midres", array_merge( [ $executable ], $resources), ATR_DENSITY_QUAD);
            }
            break;
    }
?>

<?php
}

//////////////////////////////////////////////////////////////////////////////////

function emit_rules_for_program_cc65($platform, $program, $resources = [], $embedded = false ) {

    $cbm = is_commodore($platform);

    $platform65 = get_platform65($platform);

    $options = '';
    if ( $platform == 'vic2024') {
        $options = '-D__24K__';
    }

    $outputPath = $program.".".$platform;

    if ( $embedded ) {
        $mainModule = find_main_module($program);
        $symbols = extract_symbols_from_sources();
        $neededFiles = [];
        build_list_of_needed_files( $symbols, $mainModule, $neededFiles );
        $neededFiles[] = $mainModule;
        $neededFiles[] = 'src/midres_control_011.c';
        $neededFiles[] = 'src/midres_data.c';
        $neededFiles[] = 'src/midres_cbm.c';
        $neededFiles[] = 'src/midres_data.c';
        $neededFiles[] = 'src/main.c';
        sort($neededFiles);
        $neededFiles = array_unique($neededFiles);
    
        $neededObjectFiles = [];
        foreach( $neededFiles as $neededFile ) {
            $neededObjectFiles[] = preg_replace(['#src/#', '#\.c$#'],['obj/'.$outputPath.'/', '.o'], $neededFile);
        }

#        foreach( $symbols as $module => $subsymbol ) {
#            print "# ".$module.":"."\n";
#            print "# \texport:".implode(",", $subsymbol['export'])."\n";
#            print "# \timport:".implode(",", $subsymbol['import'])."\n";
#            print "#"."\n";
#        }
    
    }

?>

# -------------------------------------------------------------------
# --- <?=strtoupper($program);?> FOR <?=strtoupper($platform);?> 
# -------------------------------------------------------------------

.PHONY: <?=$program;?>.embedded.<?=$platform;?>

<?=$program;?>.embedded.<?=$platform;?>:
	$(FILE2INCLUDE) <?php
    foreach( $resources as $key => $resource ) {
        if ( isset($resource['loader'] ) ) continue;
        if ( !isset($resource['source']) ) continue;
        $source = $resource['source'];
        $destination = $resource['destination'];
?>-i <?=$source;?> -n <?=$destination;?> <?php 
    
    }
    ?>-c src/rawdata.c -h src/rawdata.h
<?php
    if ( $embedded ) {
?>	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl <?=$cbm?'-D__CBM__':'';?> -o obj/<?=$outputPath;?>/rawdata.o src/rawdata.c

obj/<?=$outputPath;?>/midres_atari_impl.o:	src/midres_atari_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$outputPath;?>/midres_atari_impl.o src/midres_atari_impl.asm

obj/<?=$outputPath;?>/midres_c64_impl.o:	src/midres_c64_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$outputPath;?>/midres_c64_impl.o src/midres_c64_impl.asm

obj/<?=$outputPath;?>/midres_plus4_impl.o:	src/midres_plus4_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$outputPath;?>/midres_plus4_impl.o src/midres_plus4_impl.asm

obj/<?=$outputPath;?>/midres_sid_impl.o:	src/midres_sid_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$outputPath;?>/midres_sid_impl.o src/midres_sid_impl.asm

obj/<?=$outputPath;?>/midres_ted_impl.o:	src/midres_ted_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$outputPath;?>/midres_ted_impl.o src/midres_ted_impl.asm

obj/<?=$outputPath;?>/midres_pokey_impl.o:	src/midres_pokey_impl.asm
	$(ASM) -t <?=$platform65;?> -oobj/<?=$outputPath;?>/midres_pokey_impl.o src/midres_pokey_impl.asm

<?php
    }
?>

obj/<?=$outputPath;?>/%.o: <?=($embedded)?'$(LIB_INCLUDES) $(LIB_SOURCES)':'';?> $(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl -D__<?=strtoupper(trim($program,'_'));?>__ <?=$cbm?'-D__CBM__':'';?> <?=$embedded?'-DMIDRES_EMBEDDED_FILES':'';?> -o $@ $(subst obj/<?=$outputPath;?>/,src/,$(@:.o=.c))

$(EXEDIR)/<?=$program;?>.<?=$platform;?>: <?=$program.'.embedded.'.$platform;?> <?=$embedded?("obj/".$outputPath."/rawdata.o obj/".$outputPath."/midres_pokey_impl.o obj/".$outputPath."/midres_ted_impl.o obj/".$outputPath."/midres_atari_impl.o obj/".$outputPath."/midres_c64_impl.o obj/".$outputPath."/midres_plus4_impl.o obj/".$outputPath."/midres_sid_impl.o"):"";?> $(subst PLATFORM,<?=$outputPath;?>,$(LIB_OBJS)) $(subst PLATFORM,<?=$outputPath;?>,$(OBJS))
	$(CC) -Ln demo<?=$platform65;?>.lbl -t <?=$platform65;?> -C cfg/<?=$platform;?>.cfg $(LDFLAGS) -m $(EXEDIR)/<?=trim($program,'_');?>.<?=$platform65;?>.map -o $(EXEDIR)/<?=$program;?>.<?=$platform;?> <?=$embedded?("obj/".$outputPath."/rawdata.o obj/".$outputPath."/midres_pokey_impl.o obj/".$outputPath."/midres_ted_impl.o obj/".$outputPath."/midres_atari_impl.o obj/".$outputPath."/midres_c64_impl.o obj/".$outputPath."/midres_plus4_impl.o obj/".$outputPath."/midres_sid_impl.o ".implode(' ', $neededObjectFiles)):" $(subst PLATFORM,".$outputPath.",$(OBJS))";?> <?=(($embedded)?'':'$(LIBDIR)/midres.'.$platform.'.lib');?>

<?php 

    switch( $platform ) {
        case 'vic2024':
        case 'plus4':
            $loader = array_shift($resources);
            $loader = [
                "destination" => "loader",
                "source" => "$(DATADIR)/".$loader['loader']
            ];
            $executable = [
                "destination" => "$(EXEDIR)/".$program,
                "source" => "$(EXEDIR)/".$program.".".$platform,
            ];
            if ( $embedded ) {
                emit_commands_for_create_1541_disk($platform, $program, [ $loader, $executable ]);
                emit_commands_for_create_1571_disk($platform, $program, [ $loader, $executable ]);
                emit_commands_for_create_1581_disk($platform, $program, [ $loader, $executable ]);
            } else {
                emit_commands_for_create_1541_disk($platform, $program, array_merge( [ $loader, $executable ] , $resources));
                emit_commands_for_create_1571_disk($platform, $program, array_merge( [ $loader, $executable ] , $resources));
                emit_commands_for_create_1581_disk($platform, $program, array_merge( [ $loader, $executable ] , $resources));
            }
            break;
        case 'c64':
        case 'vic20':
        case 'c16':        
        case 'c128':
            $executable = [
                "destination" => "$(EXEDIR)/".$program,
                "source" => "$(EXEDIR)/".$program.".".$platform,
            ];

            if ( $embedded ) {
                emit_commands_for_create_1541_disk($platform, $program, [ $executable ]);
                emit_commands_for_create_1571_disk($platform, $program, [ $executable ]);
                emit_commands_for_create_1581_disk($platform, $program, [ $executable ]);
            } else {
                if ( isset( $resources['disks'] ) ) {
                    $disks = $resources['disks'];
                    unset($resources['disks']);
                    if ( isset($disks['1541']) ) {
                        emit_commands_for_create_1541_disk($platform, $program, array_merge([ $executable ], $resources));
                    }
                    if ( isset($disks['1571']) ) {
                        emit_commands_for_create_1571_disk($platform, $program, array_merge([ $executable ], $resources));
                    }
                    if ( isset($disks['1581']) ) {
                        emit_commands_for_create_1581_disk($platform, $program, array_merge([ $executable ], $resources));
                    }
                } else {
                    emit_commands_for_create_1541_disk($platform, $program, array_merge([ $executable ], $resources));
                    emit_commands_for_create_1571_disk($platform, $program, array_merge([ $executable ], $resources));
                    emit_commands_for_create_1581_disk($platform, $program, array_merge([ $executable ], $resources));
                }
            }
            break;
        case 'atari':
        case 'atarilo':
            $executable = [
                "destination" => "$(EXEDIR)/game.exe",
                "source" => "$(EXEDIR)/".$program.".".$platform
            ];
            if ( $embedded ) {
                emit_commands_for_create_atr_disk($platform, $program, [ $executable ], ATR_DENSITY_SINGLE);
                emit_commands_for_create_atr_disk($platform, $program, [ $executable ], ATR_DENSITY_EXTENDED);
                // emit_commands_for_create_atr_disk($platform, $program, [ $executable ], ATR_DENSITY_DOUBLE);
                // emit_commands_for_create_atr_disk($platform, $program, [ $executable ], ATR_DENSITY_QUAD);
            } else {
                emit_commands_for_create_atr_disk($platform, $program, array_merge( [ $executable ] , $resources), ATR_DENSITY_SINGLE);
                emit_commands_for_create_atr_disk($platform, $program, array_merge( [ $executable ] , $resources), ATR_DENSITY_EXTENDED);
                // emit_commands_for_create_atr_disk($platform, $program, array_merge( [ $executable ] , $resources), ATR_DENSITY_DOUBLE);
                // emit_commands_for_create_atr_disk($platform, $program, array_merge( [ $executable ] , $resources), ATR_DENSITY_QUAD);
            }
            break;
    }
?>

<?php
}

//////////////////////////////////////////////////////////////////////////////////

function emit_rules_for_ancillary_z88dk($platform, $demo, $resources = [] ) {

    $outputPath = $platform;
    $platform88 = get_platform88($platform);
    $options = '';
    $subtype = '';
    $appMakeExtension = '';

    switch( $platform ) {
        case 'msxc':
            $options = '-DGRAPHIC_MODE_II';
            $outputFormat = 'rom';
            $subtype = '-subtype=rom';
            $appMakeExtension = 'rom';
            break;
        case 'msx':
            $options = '-DGRAPHIC_MODE_I -DFRAME_BUFFER';
            $outputFormat = 'rom';
            $subtype = '-subtype=rom';
            $appMakeExtension = 'rom';
            break;
        case 'coleco':
            $options = '-DGRAPHIC_MODE_I';
            $outputFormat = 'rom';
            $appMakeExtension = 'rom';
            break;
        case 'svi':
            $options = '-DGRAPHIC_MODE_I';
            $outputFormat = 'cas';
            $appMakeExtension = 'cas';
            break;
        case 'mtx500':
            $options = '-DGRAPHIC_MODE_I -DFRAME_BUFFER';
            $outputFormat = [ 'wav', 'mtx' ];
            $appMakeExtension = [ 'wav', 'mtx' ];
            break;
        case 'gb':
            $options = '';
            $outputFormat = 'gb';
            $appMakeExtension = 'gb';
            break;
        case 'lm80c':
            $options = '-DGRAPHIC_MODE_I -DFRAME_BUFFER';
            $outputFormat = 'prg';
            $subtype = '';
            $appMakeExtension = 'PRG';
            break;
    }

    $mainModule = find_main_module("DEMO_".$demo);
    $symbols = extract_symbols_from_sources();
    $neededFiles = [];
    build_list_of_needed_files( $symbols, $mainModule, $neededFiles );
    $neededFiles[] = $mainModule;
    $neededFiles[] = 'src/midres_control_011.c';

    sort($neededFiles);
    $neededFiles = array_unique($neededFiles);
    $neededFiles[] = 'src/midres_data.c';
    $neededFiles[] = 'src/main.c';

    $neededObjectFiles = [];
    foreach( $neededFiles as $neededFile ) {
        $neededObjectFiles[] = preg_replace(['#src/#', '#\.c$#'],['obj/'.$outputPath.'/', '.o'], $neededFile);
    }

?>

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR <?=strtoupper($platform);?> 
# -------------------------------------------------------------------

.PHONY: midres.<?=$demo;?>.embedded.<?=$platform;?> midres.<?=$demo;?>.<?=$platform;?>

midres.<?=$demo;?>.embedded.<?=$platform;?>:
	$(FILE2INCLUDE) <?php
    foreach( $resources as $resource ) {
        if ( isset($resource['loader'] ) ) continue;
        $source = $resource['source'];
        $destination = $resource['destination'];
?>-i <?=$source;?> -n <?=$destination;?> <?php 
    }
    ?>-c src/rawdata.c -h src/rawdata.h
	$(CC88) +<?=$platform88;?> $(CFLAGS) -c $(CFLAGS88) <?=$options;?> -o obj/<?=$platform;?>/rawdata.o src/rawdata.c

obj/<?=$platform;?>/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/<?=$platform;?>/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/<?=$platform;?>/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/<?=$platform;?>/midres_io.o src/midres_io.asm

obj/<?=$platform;?>/%.o:	$(SOURCES)
	$(CC88) +<?=$platform88;?> $(CFLAGS) -c $(CFLAGS88) <?=$options;?> -o $@ $(subst obj/<?=$platform;?>/,src/,$(@:.o=.c))

$(EXEDIR)/midres.<?=$demo;?>.<?=$platform;?>:	midres.<?=$demo;?>.embedded.<?=$platform;?> $(subst PLATFORM,<?=$platform;?>,$(OBJS)) $(subst PLATFORM,<?=$platform;?>,$(LIB_OBJS)) obj/<?=$platform;?>/rawdata.o obj/<?=$platform;?>/midres_vdp_impl.o obj/<?=$platform;?>/midres_io.o
	$(CC88) +<?=$platform88;?> <?=$subtype;?> -m $(LDFLAGS88) obj/<?=$platform;?>/rawdata.o obj/<?=$platform;?>/midres_io.o obj/<?=$platform;?>/midres_vdp_impl.o <?=implode(' ', $neededObjectFiles);?> -create-app 
	$(call COPYFILES,a.<?=$appMakeExtension;?>,$(EXEDIR)/midres.<?=$platform;?>.<?=$outputFormat;?>)
	$(call RMFILES,a.*)
<?php
}

//////////////////////////////////////////////////////////////////////////////////

function emit_rules_for_program_z88dk($platform, $program, $resources = [] ) {

    $outputPath = $program.".".$platform;
    $platform88 = get_platform88($platform);
    $options = '';
    $subtype = '';
    $appMakeExtension = '';

    switch( $platform ) {
        case 'msxc':
            $options = '-DGRAPHIC_MODE_II';
            $outputFormat = 'rom';
            $subtype = '-subtype=rom';
            $appMakeExtension = 'rom';
            break;
        case 'msx':
            $options = '-DGRAPHIC_MODE_I -DFRAME_BUFFER';
            $outputFormat = 'rom';
            $subtype = '-subtype=rom';
            $appMakeExtension = 'rom';
            break;
        case 'coleco':
            $options = '-DGRAPHIC_MODE_I';
            $outputFormat = 'rom';
            $appMakeExtension = 'rom';
            break;
        case 'svi':
            $options = '-DGRAPHIC_MODE_I';
            $outputFormat = 'cas';
            $appMakeExtension = 'cas';
            break;
        case 'mtx500':
            $options = '-DGRAPHIC_MODE_I -DFRAME_BUFFER';
            $outputFormat = [ 'wav', 'mtx' ];
            $appMakeExtension = [ 'wav', 'mtx' ];
            break;
        case 'gb':
            $options = '';
            $outputFormat = 'gb';
            $appMakeExtension = 'gb';
            break;
        case 'lm80c':
            $options = '-DGRAPHIC_MODE_I -DFRAME_BUFFER';
            $outputFormat = 'prg';
            $subtype = '';
            $appMakeExtension = 'PRG';
            break;
    }

    $mainModule = find_main_module($program);
    $symbols = extract_symbols_from_sources();
    $neededFiles = [];
    build_list_of_needed_files( $symbols, $mainModule, $neededFiles );
    $neededFiles[] = $mainModule;

    sort($neededFiles);
    $neededFiles = array_unique($neededFiles);
    $neededFiles[] = 'src/midres_control_011.c';
    $neededFiles[] = 'src/midres_data.c';
    $neededFiles[] = 'src/main.c';

    $neededObjectFiles = [];
    foreach( $neededFiles as $neededFile ) {
        $neededObjectFiles[] = preg_replace(['#src/#', '#\.c$#'],['obj/'.$outputPath.'/', '.o'], $neededFile);
    }

?>

# -------------------------------------------------------------------
# --- <?=strtoupper($program);?> FOR <?=strtoupper($platform);?> 
# -------------------------------------------------------------------
# 
<?php

    // print "#---> ".$mainModule."\n";
    // foreach( $neededFiles as $neededFile ) {
    //     print "# ".$neededFile."\n";
    // }

//    foreach( $symbols as $module => $subsymbol ) {
//        print "# ".$module.":"."\n";
//        print "# \texport:".implode(",", $subsymbol['export'])."\n";
//        print "# \timport:".implode(",", $subsymbol['import'])."\n";
//        print "#"."\n";
//    }
?>

.PHONY: <?=$program;?>.embedded.<?=$platform;?>

<?=$program;?>.embedded.<?=$platform;?>:
	$(FILE2INCLUDE) <?php
    foreach( $resources as $resource ) {
        if ( isset($resource['loader'] ) ) continue;
        $source = $resource['source'];
        $destination = $resource['destination'];
?>-i <?=$source;?> -n <?=$destination;?> <?php 
    }
    ?>-c src/rawdata.c -h src/rawdata.h
	$(CC88) +<?=$platform88;?> $(CFLAGS) -c $(CFLAGS88) <?=$options;?> -o obj/<?=$outputPath;?>/rawdata.o src/rawdata.c

obj/<?=$outputPath;?>/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/<?=$outputPath;?>/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/<?=$outputPath;?>/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/<?=$outputPath;?>/midres_io.o src/midres_io.asm

obj/<?=$outputPath;?>/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +<?=$platform88;?> $(CFLAGS) -c $(CFLAGS88) <?=$options;?> -D__<?=strtoupper($program);?>__ -o $@ $(subst obj/<?=$outputPath;?>/,src/,$(@:.o=.c)) 

$(EXEDIR)/<?=$program;?>.<?=$platform;?>:	<?=$program;?>.embedded.<?=$platform;?> <?=implode(' ', $neededObjectFiles);?> obj/<?=$outputPath;?>/rawdata.o obj/<?=$outputPath;?>/midres_vdp_impl.o obj/<?=$outputPath;?>/midres_io.o
	$(CC88) +<?=$platform88;?> <?=$subtype;?> -m $(LDFLAGS88) obj/<?=$outputPath;?>/rawdata.o obj/<?=$outputPath;?>/midres_io.o obj/<?=$outputPath;?>/midres_vdp_impl.o <?=implode(' ', $neededObjectFiles);?> -create-app 
<?php
    if ( is_array($appMakeExtension) ) {
        foreach( $appMakeExtension as $extension ) {
?>
	$(call COPYFILES,a.<?=$extension;?>,$(EXEDIR)/<?=$program;?>.<?=$platform;?>.<?=$extension;?>)
<?php
        }
?>
<?php
    } else {
?>
	$(call COPYFILES,a.<?=$appMakeExtension;?>,$(EXEDIR)/<?=$program;?>.<?=$platform;?>.<?=$outputFormat;?>)
<?php
    }
?>
	$(call RMFILES,a.*)

<?php
}



?>