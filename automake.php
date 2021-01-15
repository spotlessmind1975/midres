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

    @include_once('automake/library.php');

    $resources = [

        'c64' => [
            [ 'destination' => 'slideshow', 'source' => '$(DATADIR)/slideshow64.dat' ],
            [ 'destination' => 'image6401.mpic', 'source' => '$(DATADIR)/image6401.mpic' ],
            [ 'destination' => 'image6402.mpic', 'source' => '$(DATADIR)/image6402.mpic' ],
            [ 'destination' => 'image6403.mpic', 'source' => '$(DATADIR)/image6403.mpic' ],
            [ 'destination' => 'image6404.mpic', 'source' => '$(DATADIR)/image6404.mpic' ],
            [ 'destination' => 'tiles.bin', 'source' => '$(DATADIR)/tiles.bin' ],
            [ 'destination' => 'aatiles.bin', 'source' => '$(DATADIR)/aatiles.bin' ],
            [ 'destination' => 'aaintro.mpic', 'source' => '$(DATADIR)/aaintro64.mpic' ],
            [ 'destination' => 'mctile.bin', 'source' => '$(DATADIR)/tutorial_mctile.bin' ],
            [ 'destination' => 'testcard.bin', 'source' => '$(DATADIR)/testcard.bin' ],
            [ 'destination' => 'zdjtiles.bin', 'source' => '$(DATADIR)/zdjtiles.bin' ],
            [ 'destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
            [ 'destination' => 'toccatina.imf', 'source' => '$(DATADIR)/toccatina.imf' ],
            [ 'destination' => 'alice.imf', 'source' => '$(DATADIR)/alice.imf' ],
            [ 'destination' => 'island.imf', 'source' => '$(DATADIR)/island.imf' ],
            [ 'destination' => 'tetris.imf', 'source' => '$(DATADIR)/tetris.imf' ]
        ],

        'vic20' => [
            [ 'destination' => 'slideshow', 'source' => '$(DATADIR)/slideshow20.dat' ],
            [ 'destination' => 'image2001.mpic', 'source' => '$(DATADIR)/image2001.mpic' ],
            [ 'destination' => 'image2002.mpic', 'source' => '$(DATADIR)/image2002.mpic' ],
            [ 'destination' => 'image2003.mpic', 'source' => '$(DATADIR)/image2003.mpic' ],
            [ 'destination' => 'image2004.mpic', 'source' => '$(DATADIR)/image2004.mpic' ],
            [ 'destination' => 'tiles.bin', 'source' => '$(DATADIR)/tiles.bin' ],
            [ 'destination' => 'aatiles.bin', 'source' => '$(DATADIR)/aatiles20.bin' ],
            [ 'destination' => 'aaintro.mpic', 'source' => '$(DATADIR)/aaintro20.mpic' ],
            [ 'destination' => 'mctile.bin', 'source' => '$(DATADIR)/tutorial_mctile.bin' ],
            [ 'destination' => 'testcard.bin', 'source' => '$(DATADIR)/testcard.bin' ],
            [ 'destination' => 'zdjtiles.bin', 'source' => '$(DATADIR)/zdjtiles.bin' ],
            [ 'destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
            [ 'destination' => 'toccatina.imf', 'source' => '$(DATADIR)/toccatina.imf' ],
            [ 'destination' => 'alice.imf', 'source' => '$(DATADIR)/alice.imf' ],
            [ 'destination' => 'island.imf', 'source' => '$(DATADIR)/island.imf' ],
            [ 'destination' => 'tetris.imf', 'source' => '$(DATADIR)/tetris.imf' ]
        ],

        'vic2024' => [
            [ 'loader' => 'loader2024.prg'],
            [ 'destination' => 'image2001.mpic', 'source' => '$(DATADIR)/image2001.mpic' ],
            [ 'destination' => 'image2002.mpic', 'source' => '$(DATADIR)/image2002.mpic' ],
            [ 'destination' => 'image2003.mpic', 'source' => '$(DATADIR)/image2003.mpic' ],
            [ 'destination' => 'image2004.mpic', 'source' => '$(DATADIR)/image2004.mpic' ],
            [ 'destination' => 'tiles.bin', 'source' => '$(DATADIR)/tiles.bin' ],
            [ 'destination' => 'aatiles.bin', 'source' => '$(DATADIR)/aatiles20.bin' ],
            [ 'destination' => 'aaintro.mpic', 'source' => '$(DATADIR)/aaintro20.mpic' ],
            [ 'destination' => 'mctile.bin', 'source' => '$(DATADIR)/tutorial_mctile.bin' ],
            [ 'destination' => 'testcard.bin', 'source' => '$(DATADIR)/testcard.bin' ],
            [ 'destination' => 'zdjtiles.bin', 'source' => '$(DATADIR)/zdjtiles.bin' ],
            [ 'destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
            [ 'destination' => 'toccatina.imf', 'source' => '$(DATADIR)/toccatina.imf' ],
            [ 'destination' => 'alice.imf', 'source' => '$(DATADIR)/alice.imf' ],
            [ 'destination' => 'island.imf', 'source' => '$(DATADIR)/island.imf' ],
            [ 'destination' => 'tetris.imf', 'source' => '$(DATADIR)/tetris.imf' ]
        ],
        

        'c16' => [
            [ 'destination' => 'slideshow', 'source' => '$(DATADIR)/slideshow16.dat' ],
            [ 'destination' => 'image1601.mpic', 'source' => '$(DATADIR)/image1601.mpic' ],
            [ 'destination' => 'image1602.mpic', 'source' => '$(DATADIR)/image1602.mpic' ],
            [ 'destination' => 'image1603.mpic', 'source' => '$(DATADIR)/image1603.mpic' ],
            [ 'destination' => 'image1604.mpic', 'source' => '$(DATADIR)/image1604.mpic' ],
            [ 'destination' => 'tiles.bin', 'source' => '$(DATADIR)/tiles.bin' ],
            [ 'destination' => 'aatiles.bin', 'source' => '$(DATADIR)/aatiles.bin' ],
            [ 'destination' => 'aaintro.mpic', 'source' => '$(DATADIR)/aaintro16.mpic' ],
            [ 'destination' => 'mctile.bin', 'source' => '$(DATADIR)/tutorial_mctile.bin' ],
            [ 'destination' => 'testcard.bin', 'source' => '$(DATADIR)/testcard.bin' ],
            [ 'destination' => 'zdjtiles.bin', 'source' => '$(DATADIR)/zdjtiles.bin' ],
            [ 'destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
            [ 'destination' => 'toccatina.imf', 'source' => '$(DATADIR)/toccatina.imf' ],
            [ 'destination' => 'alice.imf', 'source' => '$(DATADIR)/alice.imf' ],
            [ 'destination' => 'island.imf', 'source' => '$(DATADIR)/island.imf' ],
            [ 'destination' => 'tetris.imf', 'source' => '$(DATADIR)/tetris.imf' ]
        ],

        'plus4' => [
            [ 'loader' => 'loader4.prg'],
            [ 'destination' => 'slideshow', 'source' => '$(DATADIR)/slideshow16.dat' ],
            [ 'destination' => 'image1601.mpic', 'source' => '$(DATADIR)/image1601.mpic' ],
            [ 'destination' => 'image1602.mpic', 'source' => '$(DATADIR)/image1602.mpic' ],
            [ 'destination' => 'image1603.mpic', 'source' => '$(DATADIR)/image1603.mpic' ],
            [ 'destination' => 'image1604.mpic', 'source' => '$(DATADIR)/image1604.mpic' ],
            [ 'destination' => 'tiles.bin', 'source' => '$(DATADIR)/tiles.bin' ],
            [ 'destination' => 'aatiles.bin', 'source' => '$(DATADIR)/aatiles4.bin' ],
            [ 'destination' => 'aaintro.mpic', 'source' => '$(DATADIR)/aaintro16.mpic' ],
            [ 'destination' => 'mctile.bin', 'source' => '$(DATADIR)/tutorial_mctile.bin' ],
            [ 'destination' => 'testcard.bin', 'source' => '$(DATADIR)/testcard.bin' ],
            [ 'destination' => 'zdjtiles.bin', 'source' => '$(DATADIR)/zdjtiles.bin' ],
            [ 'destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
            [ 'destination' => 'toccatina.imf', 'source' => '$(DATADIR)/toccatina.imf' ],
            [ 'destination' => 'alice.imf', 'source' => '$(DATADIR)/alice.imf' ],
            [ 'destination' => 'island.imf', 'source' => '$(DATADIR)/island.imf' ],
            [ 'destination' => 'tetris.imf', 'source' => '$(DATADIR)/tetris.imf' ]
        ],

        'atari' => [
            [ 'destination' => 'slideshow', 'source' => '$(DATADIR)/slideshowa.dat' ],
            [ 'destination' => 'imagea01.mpic', 'source' => '$(DATADIR)/imagea01.pic' ],
            [ 'destination' => 'imagea02.mpic', 'source' => '$(DATADIR)/imagea02.pic' ],
            [ 'destination' => 'imagea03.mpic', 'source' => '$(DATADIR)/imagea03.pic' ],
            [ 'destination' => 'imagea04.mpic', 'source' => '$(DATADIR)/imagea04.pic' ],
            [ 'destination' => 'ztiles.bin', 'source' => '$(DATADIR)/ztiles.bin' ],
            [ 'destination' => 'tiles.bin', 'source' => '$(DATADIR)/tiles.bin' ],
            [ 'destination' => 'mctile.bin', 'source' => '$(DATADIR)/tutorial_mctile.bin' ],
            [ 'destination' => 'testcard.bin', 'source' => '$(DATADIR)/testcard.bin' ],
            [ 'destination' => 'zdjtiles.bin', 'source' => '$(DATADIR)/zdjtiles.bin' ],
            [ 'destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
            [ 'destination' => 'toccatina.imf', 'source' => '$(DATADIR)/toccatina.imf' ],
            /*[ 'destination' => 'alice.imf', 'source' => '$(DATADIR)/alice.imf' ],*/
            [ 'destination' => 'island.imf', 'source' => '$(DATADIR)/island.imf' ],
            [ 'destination' => 'tetris.imf', 'source' => '$(DATADIR)/tetris.imf' ]
        ],

        'coleco' => [
            [ 'destination' => 'mtiles.bin', 'source' => '$(DATADIR)/mtiles.bin' ],
            [ 'destination' => 'tiles.bin', 'source' => '$(DATADIR)/tiles.bin' ],
            [ 'destination' => 'tutorial_mctile.bin', 'source' => '$(DATADIR)/tutorial_mctile.bin' ],
            [ 'destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
            [ 'destination' => 'toccatina.imf', 'source' => '$(DATADIR)/toccatina.imf' ],
            [ 'destination' => 'alice.imf', 'source' => '$(DATADIR)/alice.imf' ],
            [ 'destination' => 'island.imf', 'source' => '$(DATADIR)/island.imf' ],
            [ 'destination' => 'tetris.imf', 'source' => '$(DATADIR)/tetris.imf' ]
        ],

        'msx' => [
            [ 'destination' => 'tetris.imf', 'source' => '$(DATADIR)/tetris.imf' ]
        ],

        'airattack' => [
            'c64' => [
                [ 'destination' => 'zztiles.bin', 'source' => '$(DATADIR)/aatiles.bin' ],
                [ 'destination' => 'zzintro.pic', 'source' => '$(DATADIR)/aaintro64.mpic'],
                ['destination' => 'zamusic.imf', 'source' => '$(DATADIR)/zamusic.imf' ],
                ['destination' => 'zamusic2.imf', 'source' => '$(DATADIR)/zamusic2.imf' ]
            ],
            'vic2024' => [
                [ 'loader' => 'airattack_loader2024.prg'],
                [ 'destination' => 'zztiles.bin', 'source' => '$(DATADIR)/aatiles20.bin' ],
                [ 'destination' => 'zzintro.pic', 'source' => '$(DATADIR)/aaintro20.mpic'],
                ['destination' => 'zamusic.imf', 'source' => '$(DATADIR)/zamusic.imf' ],
                ['destination' => 'zamusic2.imf', 'source' => '$(DATADIR)/zamusic2.imf' ]
            ],
            'plus4' => [
                [ 'loader' => 'airattack_loader4.prg'],
                [ 'destination' => 'zztiles.bin', 'source' => '$(DATADIR)/aatiles4.bin' ],
                [ 'destination' => 'zzintro.pic', 'source' => '$(DATADIR)/aaintro16.mpic'],
                ['destination' => 'zamusic.imf', 'source' => '$(DATADIR)/zamusic.imf' ],
                ['destination' => 'zamusic2.imf', 'source' => '$(DATADIR)/zamusic2.imf' ]
            ],
            'c128' => [
                [ 'destination' => 'zztiles.bin', 'source' => '$(DATADIR)/aatiles.bin' ],
                [ 'destination' => 'zzintro.pic', 'source' => '$(DATADIR)/aaintro64.mpic'],
                ['destination' => 'zamusic.imf', 'source' => '$(DATADIR)/zamusic.imf' ],
                ['destination' => 'zamusic2.imf', 'source' => '$(DATADIR)/zamusic2.imf' ]
            ],
            'atari' => [
                [ 'destination' => 'zztiles.bin', 'source' => '$(DATADIR)/aatiles4.bin' ],
                [ 'destination' => 'zzintro.pic', 'source' => '$(DATADIR)/aaintroa.pic'],
                ['destination' => 'zamusic.imf', 'source' => '$(DATADIR)/zamusic.imf' ],
                ['destination' => 'zamusic2.imf', 'source' => '$(DATADIR)/zamusic2.imf' ]
            ],
            'msx' => [
                [ 'destination' => 'ztiles.bin', 'source' => '$(DATADIR)/ztiles.bin' ],
                [ 'destination' => 'zztiles.bin', 'source' => '$(DATADIR)/aatiles4.bin' ],
                [ 'destination' => 'zzintro.pic', 'source' => '$(DATADIR)/aaintrox.pic'],
                ['destination' => 'zamusic.imf', 'source' => '$(DATADIR)/zamusic.imf' ],
                ['destination' => 'zamusic2.imf', 'source' => '$(DATADIR)/zamusic2.imf' ]
            ],
            'gb' => [
                [ 'destination' => 'ztiles.bin', 'source' => '$(DATADIR)/ztiles.bin' ],
                [ 'destination' => 'zztiles.bin', 'source' => '$(DATADIR)/aatiles4.bin' ],
                [ 'destination' => 'zzintro.pic', 'source' => '$(DATADIR)/aaintrox.pic']
            ],
            'lm80c' => [
                [ 'destination' => 'ztiles.bin', 'source' => '$(DATADIR)/ztiles.bin' ],
                [ 'destination' => 'zztiles.bin', 'source' => '$(DATADIR)/aatiles4.bin' ],
                [ 'destination' => 'zzintro.pic', 'source' => '$(DATADIR)/aaintrox.pic'],
                ['destination' => 'zamusic.imf', 'source' => '$(DATADIR)/zamusic.imf' ],
                ['destination' => 'zamusic2.imf', 'source' => '$(DATADIR)/zamusic2.imf' ]
            ],
        ],

        'totto' => [
            'c64' => [
                ['destination' => 'tttiles.bin', 'source' => '$(DATADIR)/tttiles.bin' ],
                ['destination' => 'tttiles1.bin', 'source' => '$(DATADIR)/tttiles1.bin' ],
                ['destination' => 'ttfinal1.mpic', 'source' => '$(DATADIR)/ttfinal164.mpic' ],
                ['destination' => 'ttfinal2.mpic', 'source' => '$(DATADIR)/ttfinal264.mpic' ],
                ['destination' => 'ttfinal3.mpic', 'source' => '$(DATADIR)/ttfinal364.mpic' ],
                ['destination' => 'ttfinal4.mpic', 'source' => '$(DATADIR)/ttfinal464.mpic' ],
                ['destination' => 'totto.imf', 'source' => '$(DATADIR)/totto.imf' ],
                ['destination' => 'totto3.imf', 'source' => '$(DATADIR)/totto3.imf' ]
            ],
            'vic2024' => [
                [ 'loader' => 'totto_loader2024.prg'],
                ['destination' => 'tttiles.bin', 'source' => '$(DATADIR)/tttiles.bin' ],
                ['destination' => 'tttiles1.bin', 'source' => '$(DATADIR)/tttiles1.bin' ],
                ['destination' => 'ttfinal1.mpic', 'source' => '$(DATADIR)/ttfinal120.mpic' ],
                ['destination' => 'ttfinal2.mpic', 'source' => '$(DATADIR)/ttfinal220.mpic' ],
                ['destination' => 'ttfinal3.mpic', 'source' => '$(DATADIR)/ttfinal320.mpic' ],
                ['destination' => 'ttfinal4.mpic', 'source' => '$(DATADIR)/ttfinal420.mpic' ],
                ['destination' => 'totto.imf', 'source' => '$(DATADIR)/totto.imf' ],
                ['destination' => 'totto3.imf', 'source' => '$(DATADIR)/totto3.imf' ]
            ],
            'plus4' => [
                [ 'loader' => 'totto_loader4.prg'],
                ['destination' => 'tttiles.bin', 'source' => '$(DATADIR)/tttiles.bin' ],
                ['destination' => 'tttiles1.bin', 'source' => '$(DATADIR)/tttiles1.bin' ],
                ['destination' => 'ttfinal1.mpic', 'source' => '$(DATADIR)/ttfinal116.mpic' ],
                ['destination' => 'ttfinal2.mpic', 'source' => '$(DATADIR)/ttfinal216.mpic' ],
                ['destination' => 'ttfinal3.mpic', 'source' => '$(DATADIR)/ttfinal316.mpic' ],
                ['destination' => 'ttfinal4.mpic', 'source' => '$(DATADIR)/ttfinal416.mpic' ],
                ['destination' => 'totto.imf', 'source' => '$(DATADIR)/totto.imf' ],
                ['destination' => 'totto3.imf', 'source' => '$(DATADIR)/totto3.imf' ]
            ],
            'msx' => [
                ['destination' => 'tttiles.bin', 'source' => '$(DATADIR)/tttiles.bin' ],
                ['destination' => 'tttiles1.bin', 'source' => '$(DATADIR)/tttiles1.bin' ],
                ['destination' => 'totto.imf', 'source' => '$(DATADIR)/totto.imf' ],
                ['destination' => 'totto3.imf', 'source' => '$(DATADIR)/totto3.imf' ]
            ],
            'gb' => [
                ['destination' => 'tttiles.bin', 'source' => '$(DATADIR)/tttiles.bin' ],
                ['destination' => 'tttiles1.bin', 'source' => '$(DATADIR)/tttiles1.bin' ]
            ],
            'lm80c' => [
                ['destination' => 'tttiles.bin', 'source' => '$(DATADIR)/tttiles.bin' ],
                ['destination' => 'tttiles1.bin', 'source' => '$(DATADIR)/tttiles1.bin' ],
                ['destination' => 'totto.imf', 'source' => '$(DATADIR)/totto.imf' ],
                ['destination' => 'totto3.imf', 'source' => '$(DATADIR)/totto3.imf' ]
            ]
        ],

        'alienstorm' => [
            'c64' => [
                ['destination' => 'zstiles.bin', 'source' => '$(DATADIR)/astiles.bin' ],
                ['destination' => 'zstiles1.bin', 'source' => '$(DATADIR)/astiles1.bin' ],
                ['destination' => 'zstiles2.bin', 'source' => '$(DATADIR)/astiles2.bin' ]
            ],
            'vic2024' => [
                [ 'loader' => 'asloader2024.prg'],
                ['destination' => 'zstiles.bin', 'source' => '$(DATADIR)/astiles.bin' ],
                ['destination' => 'zstiles1.bin', 'source' => '$(DATADIR)/astiles1.bin' ],
                ['destination' => 'zstiles2.bin', 'source' => '$(DATADIR)/astiles2.bin' ]
            ],
            'plus4' => [
                [ 'loader' => 'asloader4.prg'],
                ['destination' => 'zstiles.bin', 'source' => '$(DATADIR)/astiles.bin' ],
                ['destination' => 'zstiles1.bin', 'source' => '$(DATADIR)/astiles1.bin' ],
                ['destination' => 'zstiles2.bin', 'source' => '$(DATADIR)/astiles2.bin' ]
            ]
        ], 

        'joycheck' => [
            'c64' => [
                ['destination' => 'zdjtiles.bin', 'source' => '$(DATADIR)/zdjtiles.bin' ]
            ],
            'vic2024' => [
                [ 'loader' => 'jcloader2024.prg'],
                ['destination' => 'zdjtiles.bin', 'source' => '$(DATADIR)/zdjtiles.bin' ]
            ],
            'plus4' => [
                [ 'loader' => 'jcloader4.prg'],
                ['destination' => 'zdjtiles.bin', 'source' => '$(DATADIR)/zdjtiles.bin' ]
            ]
        ], 

        'elevator' => [
            'c64' => [
                ['destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
                ['destination' => 'zelintro.bin', 'source' => '$(DATADIR)/elevator64.mpic' ],
                ['destination' => 'elevator1.imf', 'source' => '$(DATADIR)/elevator1.imf' ]
                ],
            'vic2024' => [
                [ 'loader' => 'elloader2024.prg'],
                ['destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
                ['destination' => 'zelintro.bin', 'source' => '$(DATADIR)/elevator20.mpic' ],
                ['destination' => 'elevator1.imf', 'source' => '$(DATADIR)/elevator1.imf' ]
            ],
            'plus4' => [
                [ 'loader' => 'elloader4.prg'],
                ['destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
                ['destination' => 'zelintro.bin', 'source' => '$(DATADIR)/elevator16.mpic' ],
                ['destination' => 'elevator1.imf', 'source' => '$(DATADIR)/elevator1.imf' ]
            ],
            'atari' => [
                ['destination' => 'zeltiles.bin', 'source' => '$(DATADIR)/zeltiles.bin' ],
                ['destination' => 'zelintro.bin', 'source' => '$(DATADIR)/elevatora.mpic' ],
                ['destination' => 'elevator1.imf', 'source' => '$(DATADIR)/elevator1.imf' ]
            ]
        ]

    ];

    $resources['c16'] = $resources['plus4'];
    $resources['c128'] = $resources['c64'];
    $resources['atarilo'] = $resources['atari'];
    $resources['msxc'] = $resources['coleco'];
    $resources['svi'] = $resources['coleco'];
    $resources['mtx500'] = $resources['coleco'];
    $resources['atmos'] = $resources['atari'];
    $resources['gb'] = $resources['coleco'];
    $resources['lm80c'] = $resources['msx'];

    $resources['airattack']['c16'] = $resources['airattack']['plus4'];
    $resources['airattack']['vic20'] = $resources['airattack']['vic2024'];
    //$resources['airattack']['coleco'] = $resources['airattack']['msx'];
    $resources['airattack']['msxc'] = $resources['airattack']['msx'];
    $resources['airattack']['svi'] = $resources['airattack']['msx'];
    $resources['airattack']['mtx500'] = $resources['airattack']['msx'];
    $resources['airattack']['atmos'] = $resources['airattack']['atari'];
    $resources['airattack']['lm80c'] = $resources['airattack']['msx'];

    $resources['totto']['c16'] = $resources['totto']['plus4'];
    $resources['totto']['vic20'] = $resources['totto']['vic2024'];
    $resources['totto']['c128'] = $resources['totto']['c64'];
    //$resources['totto']['coleco'] = $resources['totto']['msx'];
    $resources['totto']['msxc'] = $resources['totto']['msx'];
    $resources['totto']['svi'] = $resources['totto']['msx'];
    $resources['totto']['mtx500'] = $resources['totto']['msx'];
    $resources['totto']['atmos'] = $resources['totto']['atari'];
    $resources['totto']['lm80c'] = $resources['totto']['msx'];

    $resources['alienstorm']['c16'] = $resources['alienstorm']['c64'];
    $resources['alienstorm']['vic20'] = $resources['alienstorm']['c64'];
    $resources['alienstorm']['c128'] = $resources['alienstorm']['c64'];
    $resources['alienstorm']['atari'] = $resources['alienstorm']['c64'];
    //$resources['alienstorm']['coleco'] = $resources['alienstorm']['c64'];
    $resources['alienstorm']['msx'] = $resources['alienstorm']['c64'];
    $resources['alienstorm']['msxc'] = $resources['alienstorm']['c64'];
    $resources['alienstorm']['svi'] = $resources['alienstorm']['c64'];
    $resources['alienstorm']['mtx500'] = $resources['alienstorm']['c64'];
    $resources['alienstorm']['atmos'] = $resources['alienstorm']['atari'];
    // $resources['alienstorm']['gb'] = $resources['alienstorm']['msx'];
    // $resources['alienstorm']['lm80c'] = $resources['alienstorm']['msx']; // it does not work?

    $resources['joycheck']['c16'] = $resources['joycheck']['plus4'];
    $resources['joycheck']['c128'] = $resources['joycheck']['c64'];
    $resources['joycheck']['atari'] = $resources['joycheck']['c64'];
    $resources['joycheck']['atmos'] = $resources['joycheck']['atari'];
    
    $resources['elevator']['c16'] = $resources['elevator']['plus4'];
    $resources['elevator']['vic20'] = $resources['elevator']['vic2024'];
    $resources['elevator']['atmos'] = $resources['elevator']['atari'];
    $resources['elevator']['msx'] = $resources['elevator']['atari'];
    $resources['elevator']['msxc'] = $resources['elevator']['c64'];
    $resources['elevator']['svi'] = $resources['elevator']['c64'];
    $resources['elevator']['mtx500'] = $resources['elevator']['c64'];
    //$resources['elevator']['coleco'] = $resources['elevator']['c64'];
    $resources['elevator']['msxc'] = $resources['elevator']['c64'];
    $resources['elevator']['c128'] = $resources['elevator']['c64'];
    $resources['elevator']['atari'] = $resources['elevator']['c64'];
    $resources['elevator']['gb'] = $resources['elevator']['msx'];
    $resources['elevator']['lm80c'] = $resources['elevator']['msx'];

    $values = file('docs/targets.md');
    $targets = [];
    foreach ( $values as $value ) {
        if ( preg_match('#- .([a-z0-9]+). -#', $value, $matched) ) {
            $value = trim($matched[1]);
            if($value) {
                $targets[] = trim($value);
            }
        }
    }
    $values = file('docs/demos.md');
    $demos = [];
    foreach ( $values as $value ) {
        if ( preg_match('#- .([A-Za-z0-9]+). -#', $value, $matched) ) {
            $demo = trim($matched[1]);
            if($demo) {
                $demos[] = trim($demo);
            }
        }
    }
    $values = file('docs/programs.md');
    $programs = [];
    foreach ( $values as $value ) {
        if ( preg_match('#- .([a-z0-9]+). - #', $value, $matched) ) {
            $program = trim($matched[1]);
            if($program) {
                $programs[] = trim($program);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////

    ob_start();

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    @include_once('automake/prologue.php');

?>

###############################################################################
## TARGETS' RULES
###############################################################################

# Makefile generated for following targets: <?=implode(', ', $targets);?>
<?php

foreach( $targets as $target ) {
        
    switch( $target ) {
        case 'msx':
        case 'msxc':
        case 'svi':
        case 'mtx500':
        case 'coleco':
        case 'gb':
        case 'lm80c':

            emit_rules_for_library_z88dk($target);
            foreach( $demos as $demo ) {
                emit_rules_for_ancillary_z88dk($target, $demo, $resources[$target] );
            }

            break;

        case 'atmos':
            emit_rules_for_library_cc65($target);
            break;

        case 'atari':
        case 'vic20':
        case 'vic2024':
        case 'c64':
        case 'c128':
        case 'c16':
        case 'plus4':
            emit_rules_for_library_cc65($target);
            foreach( $demos as $demo ) {
                emit_rules_for_ancillary_cc65($target, $demo, $resources[$target] );
            }
    }
}

foreach( $targets as $target ) {
        
    foreach( $programs as $program ) {
        switch( $target ) {
            case 'msx':
            case 'msxc':
            case 'svi':
            case 'mtx500':
            case 'coleco':
            case 'gb':
            case 'lm80c':
    
                if ( isset( $resources[$program][$target]) ) {
                    emit_rules_for_program_z88dk($target,$program,$resources[$program][$target]);
                } else {
?>                    
#!!! missing resources for <?=$program;?> (<?=$target;?>)

<?php                    
                }
                break;
    
            case 'atmos':
                break;
    
            case 'atari':
            case 'vic20':
            case 'vic2024':
            case 'c64':
            case 'c128':
            case 'c16':
            case 'plus4':
                if ( isset( $resources[$program][$target]) ) {
                    emit_rules_for_program_cc65($target, $program, $resources[$program][$target] );
                } else {
?>                    
#!!! missing resources for <?=$program;?> (<?=$target;?>)

<?php                    
                }
                break;
        }
    
    }
}

@include_once('automake/epilogue.php');

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

$content = ob_get_contents();
ob_end_clean();

file_put_contents("makefile", $content);

?>