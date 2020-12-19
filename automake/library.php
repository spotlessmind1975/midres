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

obj/<?=$platform;?>/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl <?=$cbm?'-D__CBM__':'';?>  -o $@ $(subst obj/<?=$platform;?>/,src/,$(@:.o=.c))

$(LIBDIR)/midres.<?=$platform;?>.lib:	$(LIB_INCLUDES) $(subst PLATFORM,<?=$platform;?>,$(LIB_OBJS))
	$(AR) r $(LIBDIR)/midres.<?=$platform;?>.lib $(subst PLATFORM,<?=$platform;?>,$(LIB_OBJS))

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
?>	$(CC1541) -f <?=$destination;?> -w <?=$source;?> $(EXEDIR)/<?=$diskImage;?> 
<?php        
    }

}

//////////////////////////////////////////////////////////////////////////////////

function emit_commands_for_create_atr_disk($platform,$filename,$files = []) {

    $diskImage = $filename.'.'.$platform.'.atr';

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
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/<?=$diskImage;?> $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/<?=$diskImage;?> -o $(EXEDIR)/<?=$diskImage;?> -f <?=str_replace('$(EXEDIR)/','',$first['destination']);?>
<?php

}

//////////////////////////////////////////////////////////////////////////////////

function emit_rules_for_ancillary_cc65($platform, $resources = [], $embedded = false ) {

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

<?php
    if ( $embedded ) {
?>

midres.embedded.<?=$platform;?>:
	$(FILE2INCLUDE) <?php
    foreach( $resources as $resource ) {
        $source = $resource['source'];
        $destination = $resource['destination'];
?>-i <?=$source;?> -n <?=$destination;?> <?php 
    }
    ?>-c src/rawdata.c -h src/rawdata.h
	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl <?=$cbm?'-D__CBM__':'';?> -o obj/<?=$platform65;?>/rawdata.o src/rawdata.c
<?php
    }
?>
obj/<?=$platform;?>/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl <?=$cbm?'-D__CBM__':'';?> -o $@ $(subst obj/<?=$platform;?>/,src/,$(@:.o=.c))

$(EXEDIR)/midres.<?=$platform;?>: $(subst PLATFORM,<?=$platform;?>,$(OBJS))
	$(CC) -Ln demo<?=$platform;?>.lbl -t <?=$platform65;?> -C cfg/<?=$platform;?>.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.<?=$platform;?>.map -o $(EXEDIR)/midres.<?=$platform;?> $(subst PLATFORM,<?=$platform;?>,$(OBJS)) $(LIBDIR)/midres.<?=$platform;?>.lib
<?php 

    switch( $platform ) {
        case 'c64':
        case 'vic20':
        case 'vic2024':
        case 'c16':
        case 'plus4':
        case 'c128':
            $executable = [
                "destination" => "midres-single",
                "source" => "$(EXEDIR)/midres.".$platform65,
            ];

            if ( $embedded ) {
                emit_commands_for_create_1541_disk($platform65, "midres", [ $executable ]);
            } else {
                emit_commands_for_create_1541_disk($platform65, "midres", array_merge([ $executable ], $resources));
            }
            break;
        case 'atari':
        case 'atarilo':
            $executable = [
                "destination" => "midres-single",
                "source" => "$(EXEDIR)/midres.exe"
            ];
            if ( $embedded ) {
                emit_commands_for_create_atr_disk($platform65, "midres", [ $executable ]);
            } else {
                emit_commands_for_create_atr_disk($platform65, "midres", array_merge( [ $executable ], $resources));
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

?>

# -------------------------------------------------------------------
# --- <?=strtoupper($program);?> FOR <?=strtoupper($platform);?> 
# -------------------------------------------------------------------

<?=$program;?>.embedded.<?=$platform;?>:
	$(FILE2INCLUDE) <?php
    foreach( $resources as $resource ) {
        $source = $resource['source'];
        $destination = $resource['destination'];
?>-i <?=$source;?> -n <?=$destination;?> <?php 
    }
    ?>-c src/rawdata.c -h src/rawdata.h
<?php
    if ( $embedded ) {
?>	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl <?=$cbm?'-D__CBM__':'';?> -o obj/<?=$outputPath;?>/rawdata.o src/rawdata.c
<?php
    }
?>

obj/<?=$outputPath;?>/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t <?=$platform65;?> -c $(CFLAGS) <?=$options;?> -Osir -Cl -D__<?=strtoupper($program);?>__ <?=$cbm?'-D__CBM__':'';?> -o $@ $(subst obj/<?=$outputPath;?>/,src/,$(@:.o=.c))

$(EXEDIR)/<?=$program;?>.<?=$platform;?>: <?=$program.'.embedded.'.$platform;?> $(subst PLATFORM,<?=$outputPath;?>,$(OBJS))
	$(CC) -Ln demo<?=$platform65;?>.lbl -t <?=$platform65;?> -C cfg/<?=$platform;?>.cfg $(LDFLAGS) -m $(EXEDIR)/<?=$program;?>.<?=$platform65;?>.map -o $(EXEDIR)/<?=$program;?>.<?=$platform;?> $(subst PLATFORM,<?=$outputPath;?>,$(OBJS)) $(LIBDIR)/midres.<?=$platform;?>.lib
<?php 

    switch( $platform ) {
        case 'c64':
        case 'vic20':
        case 'vic2024':
        case 'c16':
        case 'plus4':
        case 'c128':
            $executable = [
                "destination" => "$(EXEDIR)/".$program,
                "source" => "$(EXEDIR)/".$program.".".$platform,
            ];

            if ( $embedded ) {
                emit_commands_for_create_1541_disk($platform, $program, [ $executable ]);
            } else {
                emit_commands_for_create_1541_disk($platform, $program, array_merge( [ $executable ] , $resources));
            }
            break;
        case 'atari':
        case 'atarilo':
            $executable = [
                "destination" => "$(EXEDIR)/game.exe",
                "source" => "$(EXEDIR)/".$program.".".$platform
            ];
            if ( $embedded ) {
                emit_commands_for_create_atr_disk($platform, "midres", [ $executable ]);
            } else {
                emit_commands_for_create_atr_disk($platform, "midres", array_merge( [ $executable ] , $resources));
            }
            break;
    }
?>

<?php
}

//////////////////////////////////////////////////////////////////////////////////

function emit_rules_for_ancillary_z88dk($platform, $resources = [] ) {

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
            $options = '-DGRAPHIC_MODE_I';
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
            $options = '-DGRAPHIC_MODE_I';
            $outputFormat = 'mtx';
            $appMakeExtension = 'mtx500.cas';
            break;
    }


?>

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR <?=strtoupper($platform);?> 
# -------------------------------------------------------------------

midres.embedded.<?=$platform;?>:
	$(FILE2INCLUDE) <?php
    foreach( $resources as $resource ) {
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

$(EXEDIR)/midres.<?=$platform;?>:	midres.embedded.<?=$platform;?> $(subst PLATFORM,<?=$platform;?>,$(OBJS)) $(subst PLATFORM,<?=$platform;?>,$(LIB_OBJS)) obj/<?=$platform;?>/rawdata.o obj/<?=$platform;?>/midres_vdp_impl.o obj/<?=$platform;?>/midres_io.o
	$(CC88) +<?=$platform88;?> <?=$subtype;?> -m $(LDFLAGS88) obj/<?=$platform;?>/rawdata.o obj/<?=$platform;?>/midres_io.o obj/<?=$platform;?>/midres_vdp_impl.o $(subst PLATFORM,<?=$platform;?>,$(LIB_OBJS)) $(subst PLATFORM,<?=$platform;?>,$(OBJS)) -o $(EXEDIR)/midres.<?=$platform;?> -create-app 
	$(call COPYFILES,$(EXEDIR)/midres.<?=$appMakeExtension;?>,$(EXEDIR)/midres.<?=$platform;?>.<?=$outputFormat;?>)
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
            $options = '-DGRAPHIC_MODE_I';
            $outputFormat = 'mtx';
            $appMakeExtension = 'mtx500.cas';
            break;
    }

?>

# -------------------------------------------------------------------
# --- <?=strtoupper($program);?> FOR <?=strtoupper($platform);?> 
# -------------------------------------------------------------------

<?=$program;?>.embedded.<?=$platform;?>:
	$(FILE2INCLUDE) <?php
    foreach( $resources as $resource ) {
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

$(EXEDIR)/<?=$program;?>.<?=$platform;?>:	<?=$program;?>.embedded.<?=$platform;?> $(subst PLATFORM,<?=$outputPath;?>,$(OBJS)) $(subst PLATFORM,<?=$outputPath;?>,$(LIB_OBJS)) obj/<?=$outputPath;?>/rawdata.o obj/<?=$outputPath;?>/midres_vdp_impl.o obj/<?=$outputPath;?>/midres_io.o
	$(CC88) +<?=$platform88;?> <?=$subtype;?> -m $(LDFLAGS88) obj/<?=$outputPath;?>/rawdata.o obj/<?=$outputPath;?>/midres_io.o obj/<?=$outputPath;?>/midres_vdp_impl.o $(subst PLATFORM,<?=$outputPath;?>,$(LIB_OBJS)) $(subst PLATFORM,<?=$outputPath;?>,$(OBJS)) -o $(EXEDIR)/<?=$program;?>.<?=$platform;?> -create-app 
	$(call COPYFILES,$(EXEDIR)/<?=$program;?>.<?=$appMakeExtension;?>,$(EXEDIR)/<?=$program;?>.<?=$platform;?>.<?=$outputFormat;?>)

<?php
}
?>