#ifndef FONT_H
#define FONT_H

#define STARTING_SYMBOL_OFFSET 33
#define FONT_SYMBOLS_NUMBER 94
#define FONT_WIDTH 10
#define FONT_HEIGHT 22

// CHANGE XPM TYPE SEE EMAIL
static const char * font_xpm[] = {
"940 22 2 1",
" 	c None",
".	c #FFFFFF",
"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ",
"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ",
"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ",
"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ",
"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ",
"    ..       .  .                  .                            ..          ..     .                                                               ..                                                                                                                                                          ....                                                                                                                                                                                                                                                                                    .....   ..          .....                         ..                ..                        ..                ....            ..           ..          .    ..         ....                                                                                                                                                              ..                        ",
"    ..       .  .      ..  .      ...     ..    .    ...        ..         ..      ..                                                              .     ....       ..       ....      ....         ..    ......      ....    .......    ....      ....                                                       ..  ..     ....       ..     ......       ....   ......    ........   .......    ....    ..    ..   ......     ......  .     ..  ..        .      .  ..     .    ....    ......      ....    ......      ....    ........  .      .  .      . ..       . ..    ..  .      .  ........    .        .              .       .                  .                ..                        ..               ..  .            ..           ..          .    ..           ..                                                                                                                                                     ...      ..     ....               ",
"    ..       .  .      .   .    ......   . ..  .    .. ..       ..        ..        ..                                                            ..    ..  ..    ....      ..  ..    ..  ..        ..    .          ..  .         .    ..  ..    ..  ..                              .            .               ..   ..   .      ..     .    ..    ..   ..  ..   ..   ..         .         ..   ..  .     ..     ..          .    .    ..   ..        ..    ..  ..     .   ..  ..   ..   ..    ..  ..   ..   ..    ..   .      ..     .      .  .      .  .   .  ..  .    .   ..    ..        ..    .        ..             .      ..                   .               ..                        ..              ..                ..                            ..           ..                                                                              .                                                                     .         ..        ..              ",
"    ..       .  .      .   .    .  .     .  . ..    .   .       ..        .          ..       ..        ..                                        .     .    .       .     ..    ..        .       ...    .         ..            ..    .    .    .    .                            ..              ..             ..  ..     .     ..     .     ..   .     .  .     .   ..         .        ..        .     ..     .           .    .   ..    ..        ..    ..  ...    .  ..    ..  ..    ..  ..    ..  ..    ..  ..           ..     .      .  ..    ..  .   .  .   ..  ..    .    .        ..     .         .             .     ....                                  ..                        ..              ..                ..                            ..           ..                                                                              .                                                                    ..         ..        ..              ",
"    ..       .  .    ........   .  .     .  . .     .   .       ..       ..           .       ..        ..                                       ..    ..    ..      .           ..        .      ....    .         .             ..    .    .   ..    ..                         ..     ........     ..           .   .   ....    ....    .     ..  ..        .     ..  ..         .        .         .     ..     .           .    .  ..     ..        ...  ...  . .    .  .      .  ..     .  .      .  ..    ..   .           ..     .      .   .    .   .  ..  .    .  .     ..  ..        .      .         ..            .     .  .                          ....    .. ...       ....     ... ..    ....    .......     ... ..  .. ...     ....       ....    ..    .      ..     . .. ...  .. ...      ....    .. ...      ......   .. ....    ....    .......   ..    ..  .      . ..      .. ..    .   ..    ..  ........    ..         ..         .              ",
"    ..                 .  ..    .. .     ... .       . ..                .            ..   .. ..  .     ..                                       .     ..   ...      .           ..       ..      . ..    .....     . ...         .     ..  ..   ..    ..     ..        ..      ..                      ..        ..   .  ..  .    .  .    .    ..   .         .      .  ..         .        .         .     ..     .           .    . ..      ..        . .  . .  . ..   .  .      .  ..     .  .      .  ..    ..   ..          ..     .      .   ..   .   .  ..  .    ....      .. .        ..      .          .            .    .   ..                        .   ..   ...  ..    ..   ..   ..  ...   ..  ..     ..       .   ..   ...  ..      ..          .    ..   .       ..     .. ... .  ...  ..    ..  ..   ...  ..    ..  ...   ...   .   ..  ..     ..      ..    ..  ..    ..  .  ..  .   ..  ..    .    .        ..     ..         ..         .              ",
"    ..                 .  .      ...         .       ...                 .             .    ......   ........                      ........     ..     ..  . ..      .           .      ...      .  ..    ..  ..   ...  ..       ..      ....     ..   ..     ..        ..     ..                        ..      ..    .  .   .    .  .    ......    .         .      .  .......    ......   .         ........     .           .    ....      ..        . ...  .  .  ..  .  .      .  ..    ..  .      .  ..    .     ...        ..     .      .   ..  ..   .. ... .     ..       ....        .       .          ..           .                                       ..  ..    ..   .        ..    ..  ..    ..    ..      ..   ..   ..    ..     ..          .    ..  .        ..     .  ..  .  ..    .   ..    ..  ..    ..  ..    ..   ..        .          ..      ..    ..   .    .   .  ..  .    .  .     .    .        .      .          ..         .      ...   . ",
"    ..                 .  .        ...      .       ....                ..             .      ..        ..                                      .      .. .  ..      .          .         ..    ..  ..         ..  ..    ..      .      .. ...     ......                      ...       ........       ...      .     .  .  ..   ..  ..   .    ..   .         .      .  ..         .        .    ...  ..    ..     .           .    .. .      ..        .  ..  .  .   .  .  .      .  .......   .      .  .......       ...      ..     .      .    .  .    ...  . .     ..        ..        .        .           .           .                                        .  ..     .  ..        .     ..  ........    ..      ..   ..   ..    ..     ..          .    ....         ..     .  ..  .  ..    ..  .      .  ..     .  .     ..   ..        ...        ..      ..    ..   ..   .   .  ..  .    ...      ..  ..       .     ..           ..          ..    .  .... ",
"    ..                 .  .        . ..    .. ...  ..  .  .             ..             .     .  .       ..                                     ..      ...   ..      .         ..          .   ..   ..         ..  ..    ..      .      .    .         ..                        ...                  ...        .     .  .....   ......   .     ..  .         .      .  ..         .        .      .  .     ..     .           .    .  ..     ..        .      .  .   .. .  .      .  ..        .      .  ..  ..          ..     ..     .      .    .. .     ..  ...    ....       ..       ..        .           ..          .                                   ......  ..     .  ..        .     ..  ..          ..       .   .    ..    ..     ..          .    .....        ..     .  ..  .  ..    ..  .      .  ..     .  .     ..   ..          ...      ..      ..    ..    .  ..   .. ... .     ..       .  .       ..      ..          ..         ..             ",
"                     ........      .  .    . .. .  .    ...              .             .    ..  ..      ..                                     .       ...   ..      .        ..           ..  ........        ..   .    ..     ..     ..    ..        .                           ..                ..                .          .    .   .      .  ..        .     ..  ..         .        ..     .  .     ..     .           .    .   ..    ..        .      .  .    ...  .      .  ..        .      .  ..   .           .     ..     .      .    ....     ..  ..     .  .       ..       .         .            .          .                                  ..   ..  ..     .  ..        .     ..  ..          ..       .....    ..    ..     ..          .    ..  ..       ..     .  ..  .  ..    ..  .      .  ..     .  .     ..   ..             ..    ..      ..    ..    .  .    ...  . .    ....      .. .      ..        ..         ..         .              ",
"                      ..  .        .  .   .  .  .. .    ..               .            ..    .    .      ..                                     .        .    .       .       ..            .        ..         ..   .    ..     .      ..    ..        .                             ..            ..                   .        ..    ..  .     ..   .        .     .   ..         .        ..     .  .     ..     .           .    .    ..   ..        .      .  .    ...  ..    ..  ..        ..    ..  ..   ..          .     ..     ..    ..     ..      ..   .    ..  ..      ..      ..         .            .          .                                 ..    ..  ..    ..   .        ..    ..  ..          ..       .        ..    ..     ..          .    ..   .       ..     .  ..  .  ..    ..  ..    ..  ..    ..  ..    ..   ..             ..    ..      ..    ..    ....     ..  ...    .  .       ...     ..         ..         ..         .              ",
"    ..                .   .    ..  . ..   .  .. .  ..  ....              ..           .                           ..        ..                ..        ..  ..       .      ..        .   ..        ..    ..  ..    ..  ..      .       ..  ..    .   .       ..        ..            .            .            ..      ...  .   .      .  .    ...   ..   ..  ..  ...   ..         .         ..   ..  .     ..     ..     ..  ..    .     .   ..        .      .  .     ..   ..  ..   ..         .    .   ..    .   ..   ..      ..      ..  ..      ..      .    .    .    .      ..     ..          .            ..         .                                 ..   ...  ...  ..    ..   ..   ..  ...   ..   .     ..       .        ..    ..     ..          .    ..   ..      ..     .  ..  .  ..    ..   ..  ..   ...  ..    ..  ...   ..       ..    .      .  ..   ..  ...     ..      ..  ..    ..  ..      ..     ..          ..         ..         .              ",
"    ..                .  ..      ....    .    ...   ....  .               .          ..                           ..        ..                .          ....        .     ........    ....         ..     ....      ....      ..        ....     ....        ..        ..                                      ..        ....  ..      .. ......       ....   .....     ........   .          .....   .     ..   ......    ....     .     ..  ........  .      .  .      .    ....    ..          .....   ..    ..   .....       ..       ....        .      .    .   ..    ..     ..     ........    .             .         .                                  .... ..  .  ...       ....     ... ..    ....      ..       ......   ..    ..   ......        .    ..    ..   ......   .  ..  .  ..    ..    ....    .. ...      ... ..   ..         ....       ....     ... ..     ..      ..   .   ..    ..      .     ........    ..         ..         .              ",
"                                   .                                      ..        ..                            ..                         ..                                                                                                                         ..                                                                                                                                                                                                                          ..                                                                                                 .....               .....             ........                                                                        .     ..                           .                                                      ..              ..                                                                           ..                 ..         ..        ..              ",
"                                                                           ..      ..                             .                                                                                                                                                     .                                                                                                                                                                                                                            .                                                                                                                                                                                                                       ..    ..                      ..  ..                                                      ..              ..                                                                           .                   ..        ..        ..              ",
"                                                                            ..    ..                              .                                                                                                                                                     .                                                                                                                                                                                                                            ....                                                                                                                                                                                                                     ......                        ....                                                       ..              ..                                                                        ...                     ...      ..     ....               ",
"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            "};

#endif
