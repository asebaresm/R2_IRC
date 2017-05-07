$doxydocs=
{
  classes => [
  ],
  namespaces => [
  ],
  files => [
    {
      name => 'G-2361-06-P1-Daemon.h',
      includes => [
        {
          name => 'stdio.h'
        },
        {
          name => 'string.h'
        },
        {
          name => 'sys/types.h'
        },
        {
          name => 'sys/stat.h'
        },
        {
          name => 'sys/socket.h'
        },
        {
          name => 'netinet/in.h'
        },
        {
          name => 'arpa/inet.h'
        },
        {
          name => 'syslog.h'
        },
        {
          name => 'stdlib.h'
        },
        {
          name => 'unistd.h'
        },
        {
          name => 'pthread.h'
        },
        {
          name => 'redes2/irc.h'
        }
      ],
      included_by => [
        {
          name => 'src/G-2361-06-P1-prueba.c',
          ref => '_g-2361-06-_p1-prueba_8c'
        },
        {
          name => 'srclib/G-2361-06-P1-Daemon.c',
          ref => '_g-2361-06-_p1-_daemon_8c'
        }
      ],
      functions => {
        members => [
          {
            kind => 'function',
            name => 'daemonizar',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'int',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'servicio',
                type => 'char *'
              }
            ]
          }
        ]
      },
      brief => {},
      detailed => {}
    },
    {
      name => 'G-2361-06-P1-Functions.h',
      includes => [
        {
          name => 'stdio.h'
        },
        {
          name => 'string.h'
        },
        {
          name => 'sys/types.h'
        },
        {
          name => 'sys/stat.h'
        },
        {
          name => 'sys/socket.h'
        },
        {
          name => 'netinet/in.h'
        },
        {
          name => 'arpa/inet.h'
        },
        {
          name => 'syslog.h'
        },
        {
          name => 'stdlib.h'
        },
        {
          name => 'unistd.h'
        },
        {
          name => 'pthread.h'
        },
        {
          name => 'redes2/irc.h'
        },
        {
          name => '../includes/G-2361-06-P1-Sockets.h',
          ref => '_g-2361-06-_p1-_sockets_8h'
        },
        {
          name => '../includes/G-2361-06-P1-Server.h',
          ref => '_g-2361-06-_p1-_server_8h'
        }
      ],
      included_by => [
        {
          name => 'srclib/G-2361-06-P1-Functions.c',
          ref => '_g-2361-06-_p1-_functions_8c'
        },
        {
          name => 'srclib/G-2361-06-P1-Server.c',
          ref => '_g-2361-06-_p1-_server_8c'
        }
      ],
      functions => {
        members => [
          {
            kind => 'function',
            name => 'liberaLista',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'lista',
                type => 'char **'
              },
              {
                declaration_name => 'nElems',
                type => 'long'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionNick',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'id',
                type => 'long'
              },
              {
                declaration_name => 'usuario',
                type => 'char *'
              },
              {
                declaration_name => 'nick_name',
                type => 'char *'
              },
              {
                declaration_name => 'real',
                type => 'char *'
              },
              {
                declaration_name => 'nick',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionUser',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'user',
                type => 'char *'
              },
              {
                declaration_name => 'nick',
                type => 'char *'
              },
              {
                declaration_name => 'realname',
                type => 'char *'
              },
              {
                declaration_name => 'modehost',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionPing',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'server1',
                type => 'char *'
              },
              {
                declaration_name => 'server2',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionAway',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'id',
                type => 'long'
              },
              {
                declaration_name => 'usuario',
                type => 'char *'
              },
              {
                declaration_name => 'nick_name',
                type => 'char *'
              },
              {
                declaration_name => 'real',
                type => 'char *'
              },
              {
                declaration_name => 'away',
                type => 'char *'
              },
              {
                declaration_name => 'msg',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionPart',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'channel',
                type => 'char *'
              },
              {
                declaration_name => 'nick_name',
                type => 'char *'
              },
              {
                declaration_name => 'msg',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionList',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'nick',
                type => 'char *'
              },
              {
                declaration_name => 'target',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionMotd',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'nick_name',
                type => 'char *'
              },
              {
                declaration_name => 'target',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              },
              {
                declaration_name => 'fich',
                type => 'char *'
              }
            ]
          }
        ]
      },
      brief => {},
      detailed => {}
    },
    {
      name => 'G-2361-06-P1-Server.h',
      includes => [
        {
          name => 'stdio.h'
        },
        {
          name => 'string.h'
        },
        {
          name => 'sys/types.h'
        },
        {
          name => 'sys/stat.h'
        },
        {
          name => 'sys/socket.h'
        },
        {
          name => 'netinet/in.h'
        },
        {
          name => 'arpa/inet.h'
        },
        {
          name => 'syslog.h'
        },
        {
          name => 'stdlib.h'
        },
        {
          name => 'unistd.h'
        },
        {
          name => 'pthread.h'
        },
        {
          name => 'redes2/irc.h'
        },
        {
          name => '../includes/G-2361-06-P1-Sockets.h',
          ref => '_g-2361-06-_p1-_sockets_8h'
        }
      ],
      included_by => [
        {
          name => 'includes/G-2361-06-P1-Functions.h',
          ref => '_g-2361-06-_p1-_functions_8h'
        },
        {
          name => 'src/G-2361-06-P1-prueba.c',
          ref => '_g-2361-06-_p1-prueba_8c'
        },
        {
          name => 'srclib/G-2361-06-P1-Server.c',
          ref => '_g-2361-06-_p1-_server_8c'
        }
      ],
      defines => {
        members => [
          {
            kind => 'define',
            name => 'PREFIJO',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Nombre del prefijo '
                }
              ]
            },
            initializer => '"servidor-P6"'
          },
          {
            kind => 'define',
            name => 'SIZE',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Tamanno para la entrada '
                }
              ]
            },
            initializer => '8192'
          },
          {
            kind => 'define',
            name => 'NUM_SOCKETS',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Numero de sockets maximo '
                }
              ]
            },
            initializer => '100'
          }
        ]
      },
      functions => {
        members => [
          {
            kind => 'function',
            name => 'manejador_alarma',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'sig',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'procesar',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'void '
                },
                {
                  type => 'url',
                  link => '_g-2361-06-_p1-_server_8h_1ad27ad83298aac3580deda730e80a62f0',
                  content => 'manejador_alarma(int sig)'
                },
                {
                  type => 'text',
                  content => '{ char *ping_msj = NULL; int i;'
                },
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'IRCMsg_Ping (&ping_msj, PREFIJO, "localhost", NULL); syslog(LOG_INFO, "Numero de usuarios %d", numeroUsuarios); for(i = 0; i < numeroUsuarios; i++){ send(sockets[i], ping_msj, strlen(ping_msj), 0); } free(ping_msj); alarm(30); } '
                }
              ]
            },
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'entrada',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              },
              {
                declaration_name => 'readset',
                type => 'fd_set'
              }
            ]
          },
          {
            kind => 'function',
            name => 'servidor',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'puerto',
                type => 'int'
              },
              {
                declaration_name => 'path',
                type => 'char *'
              }
            ]
          },
          {
            kind => 'function',
            name => 'parseCommand',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'query',
                type => 'long int'
              },
              {
                declaration_name => 'comando',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          }
        ]
      },
      brief => {},
      detailed => {}
    },
    {
      name => 'G-2361-06-P1-Sockets.h',
      includes => [
        {
          name => 'stdio.h'
        },
        {
          name => 'string.h'
        },
        {
          name => 'sys/types.h'
        },
        {
          name => 'sys/stat.h'
        },
        {
          name => 'sys/socket.h'
        },
        {
          name => 'netinet/in.h'
        },
        {
          name => 'arpa/inet.h'
        },
        {
          name => 'syslog.h'
        },
        {
          name => 'stdlib.h'
        },
        {
          name => 'unistd.h'
        },
        {
          name => 'pthread.h'
        },
        {
          name => 'redes2/irc.h'
        }
      ],
      included_by => [
        {
          name => 'includes/G-2361-06-P1-Functions.h',
          ref => '_g-2361-06-_p1-_functions_8h'
        },
        {
          name => 'includes/G-2361-06-P1-Server.h',
          ref => '_g-2361-06-_p1-_server_8h'
        },
        {
          name => 'srclib/G-2361-06-P1-Sockets.c',
          ref => '_g-2361-06-_p1-_sockets_8c'
        }
      ],
      defines => {
        members => [
          {
            kind => 'define',
            name => 'NUM_SOCKETS',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            initializer => '100'
          },
          {
            kind => 'define',
            name => 'SIZE',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            initializer => '8192'
          }
        ]
      },
      functions => {
        members => [
          {
            kind => 'function',
            name => 'abrir_socketTCP',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'int',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'puerto',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'acepta_conexion',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'int',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          }
        ]
      },
      brief => {},
      detailed => {}
    },
    {
      name => 'G-2361-06-P1-prueba.c',
      includes => [
        {
          name => 'stdio.h'
        },
        {
          name => 'string.h'
        },
        {
          name => 'sys/types.h'
        },
        {
          name => 'sys/stat.h'
        },
        {
          name => 'sys/socket.h'
        },
        {
          name => 'netinet/in.h'
        },
        {
          name => 'arpa/inet.h'
        },
        {
          name => 'syslog.h'
        },
        {
          name => 'stdlib.h'
        },
        {
          name => 'unistd.h'
        },
        {
          name => 'pthread.h'
        },
        {
          name => 'redes2/irc.h'
        },
        {
          name => '../includes/G-2361-06-P1-Daemon.h',
          ref => '_g-2361-06-_p1-_daemon_8h'
        },
        {
          name => '../includes/G-2361-06-P1-Server.h',
          ref => '_g-2361-06-_p1-_server_8h'
        }
      ],
      included_by => [
      ],
      functions => {
        members => [
          {
            kind => 'function',
            name => 'main',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'int',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'argc',
                type => 'int'
              },
              {
                declaration_name => 'argv',
                type => 'char *',
                array => '[]'
              }
            ]
          }
        ]
      },
      brief => {
        doc => [
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Modulo main. '
          }
        ]
      },
      detailed => {
        doc => [
          {
            author => [
              {
                type => 'text',
                content => 'Alfonso Sebares '
              },
              {
                type => 'parbreak'
              },
              {
                type => 'text',
                content => 'Beatriz de Pablo '
              },
              {
                type => 'parbreak'
              },
              {
                type => 'text',
                content => 'Celia Mateos '
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13/02/17 '
              }
            ]
          }
        ]
      }
    },
    {
      name => 'G-2361-06-P1-Daemon.c',
      includes => [
        {
          name => '../includes/G-2361-06-P1-Daemon.h',
          ref => '_g-2361-06-_p1-_daemon_8h'
        }
      ],
      included_by => [
      ],
      functions => {
        members => [
          {
            kind => 'function',
            name => 'daemonizar',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'int',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'servicio',
                type => 'char *'
              }
            ]
          }
        ]
      },
      brief => {},
      detailed => {}
    },
    {
      name => 'G-2361-06-P1-Functions.c',
      includes => [
        {
          name => '../includes/G-2361-06-P1-Functions.h',
          ref => '_g-2361-06-_p1-_functions_8h'
        }
      ],
      included_by => [
      ],
      functions => {
        members => [
          {
            kind => 'function',
            name => 'liberaLista',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'lista',
                type => 'char **'
              },
              {
                declaration_name => 'nElems',
                type => 'long'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionUser',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'user',
                type => 'char *'
              },
              {
                declaration_name => 'nick',
                type => 'char *'
              },
              {
                declaration_name => 'realname',
                type => 'char *'
              },
              {
                declaration_name => 'modehost',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionNick',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'id',
                type => 'long'
              },
              {
                declaration_name => 'usuario',
                type => 'char *'
              },
              {
                declaration_name => 'nick_name',
                type => 'char *'
              },
              {
                declaration_name => 'real',
                type => 'char *'
              },
              {
                declaration_name => 'nick',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionAway',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'id',
                type => 'long'
              },
              {
                declaration_name => 'usuario',
                type => 'char *'
              },
              {
                declaration_name => 'nick_name',
                type => 'char *'
              },
              {
                declaration_name => 'real',
                type => 'char *'
              },
              {
                declaration_name => 'away',
                type => 'char *'
              },
              {
                declaration_name => 'msg',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionPing',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'server1',
                type => 'char *'
              },
              {
                declaration_name => 'server2',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionPart',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'channel',
                type => 'char *'
              },
              {
                declaration_name => 'nick_name',
                type => 'char *'
              },
              {
                declaration_name => 'msg',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionList',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'nick',
                type => 'char *'
              },
              {
                declaration_name => 'target',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'funcionMotd',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'nick_name',
                type => 'char *'
              },
              {
                declaration_name => 'target',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              },
              {
                declaration_name => 'fich',
                type => 'char *'
              }
            ]
          }
        ]
      },
      brief => {},
      detailed => {}
    },
    {
      name => 'G-2361-06-P1-Server.c',
      includes => [
        {
          name => '../includes/G-2361-06-P1-Server.h',
          ref => '_g-2361-06-_p1-_server_8h'
        },
        {
          name => '../includes/G-2361-06-P1-Functions.h',
          ref => '_g-2361-06-_p1-_functions_8h'
        }
      ],
      included_by => [
      ],
      functions => {
        members => [
          {
            kind => 'function',
            name => 'procesar',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'void '
                },
                {
                  type => 'url',
                  link => '_g-2361-06-_p1-_server_8h_1ad27ad83298aac3580deda730e80a62f0',
                  content => 'manejador_alarma(int sig)'
                },
                {
                  type => 'text',
                  content => '{ char *ping_msj = NULL; int i;'
                },
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'IRCMsg_Ping (&ping_msj, PREFIJO, "localhost", NULL); syslog(LOG_INFO, "Numero de usuarios %d", numeroUsuarios); for(i = 0; i < numeroUsuarios; i++){ send(sockets[i], ping_msj, strlen(ping_msj), 0); } free(ping_msj); alarm(30); } '
                }
              ]
            },
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'entrada',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              },
              {
                declaration_name => 'readset',
                type => 'fd_set'
              }
            ]
          },
          {
            kind => 'function',
            name => 'servidor',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'puerto',
                type => 'int'
              },
              {
                declaration_name => 'path',
                type => 'char *'
              }
            ]
          },
          {
            kind => 'function',
            name => 'parseCommand',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'void',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'query',
                type => 'long int'
              },
              {
                declaration_name => 'comando',
                type => 'char *'
              },
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          }
        ]
      },
      variables => {
        members => [
          {
            kind => 'variable',
            name => 'user_mutex',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Se crea e inicializa el mutex para los usuarios '
                }
              ]
            },
            type => 'pthread_mutex_t',
            initializer => '= PTHREAD_MUTEX_INITIALIZER'
          },
          {
            kind => 'variable',
            name => 'canal_mutex',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Se crea e inicializa el mutex para los canales '
                }
              ]
            },
            type => 'pthread_mutex_t',
            initializer => '= PTHREAD_MUTEX_INITIALIZER'
          },
          {
            kind => 'variable',
            name => 'fich',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena la ruta del fichero motd.txt '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'prefix',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena el prefijo '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'nick',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nick del usuario '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'msg',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el mensaje al realizar el parseo de algunos comandos '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'user',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nombre del usuario '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'modehost',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena el modo del host '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'serverother',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nombre del servidor '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'realname',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el realname del usuario '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'server1',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nombre del servidor 1 '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'server2',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nombre del servidor 2 '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'channel',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nombre del canal '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'key',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena la clave de un canal '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'target',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena la mascara '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'maskarray',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena la mascara de un usuario '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'msgtarget',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nick del receptor de un mensaje o el nombre de un canal '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'topic',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el topic de un canal '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'channeluser',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el canal del usuario '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'modo',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena el modo de un canal '
                }
              ]
            },
            type => 'char *'
          },
          {
            kind => 'variable',
            name => 'real',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nombre real de un usuario '
                }
              ]
            },
            type => 'char *',
            initializer => '= NULL'
          },
          {
            kind => 'variable',
            name => 'ip',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena la ip de un usuario '
                }
              ]
            },
            type => 'char *',
            initializer => '= NULL'
          },
          {
            kind => 'variable',
            name => 'away',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el estado del usuario '
                }
              ]
            },
            type => 'char *',
            initializer => '= NULL'
          },
          {
            kind => 'variable',
            name => 'host',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena el host de un usuario '
                }
              ]
            },
            type => 'char *',
            initializer => '= NULL'
          },
          {
            kind => 'variable',
            name => 'nick_name',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nick del usuario '
                }
              ]
            },
            type => 'char *',
            initializer => '= NULL'
          },
          {
            kind => 'variable',
            name => 'usuario',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable donde se almacena el nombre del usuario '
                }
              ]
            },
            type => 'char *',
            initializer => '= NULL'
          },
          {
            kind => 'variable',
            name => 'listaNicks',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable donde se almacenan los nicks de los usuarios '
                }
              ]
            },
            type => 'char',
            arguments => '[NUM_SOCKETS][10]'
          },
          {
            kind => 'variable',
            name => 'accion',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable que almacena la accion de un usuario '
                }
              ]
            },
            type => 'long',
            initializer => '= 0'
          },
          {
            kind => 'variable',
            name => 'creacion',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'Variable que almacena la creacion de un usuario '
                }
              ]
            },
            type => 'long',
            initializer => '= 0'
          },
          {
            kind => 'variable',
            name => 'id',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable que almacena el id del usuario '
                }
              ]
            },
            type => 'long',
            initializer => '= 0'
          },
          {
            kind => 'variable',
            name => 'socketAlrm',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable que almacena el id del socket '
                }
              ]
            },
            type => 'int',
            initializer => '= 0'
          },
          {
            kind => 'variable',
            name => 'numeroUsuarios',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable que almacena el numero de usuarios '
                }
              ]
            },
            type => 'int',
            initializer => '= 0'
          },
          {
            kind => 'variable',
            name => 'sockets',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {
              doc => [
                {
                  type => 'text',
                  content => 'Variable que almacena el numero de socket de cada usuario '
                }
              ]
            },
            type => 'int',
            arguments => '[NUM_SOCKETS]'
          }
        ]
      },
      brief => {
        doc => [
          {
            type => 'text',
            content => 'Implementacion de las funciones del Servidor. '
          }
        ]
      },
      detailed => {
        doc => [
          {
            author => [
              {
                type => 'text',
                content => 'Alfonso Sebares '
              },
              {
                type => 'parbreak'
              },
              {
                type => 'text',
                content => 'Beatriz de Pablo '
              },
              {
                type => 'parbreak'
              },
              {
                type => 'text',
                content => 'Celia Mateos '
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13/02/17 '
              }
            ]
          }
        ]
      }
    },
    {
      name => 'G-2361-06-P1-Sockets.c',
      includes => [
        {
          name => '../includes/G-2361-06-P1-Sockets.h',
          ref => '_g-2361-06-_p1-_sockets_8h'
        }
      ],
      included_by => [
      ],
      functions => {
        members => [
          {
            kind => 'function',
            name => 'abrir_socketTCP',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'int',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'puerto',
                type => 'int'
              }
            ]
          },
          {
            kind => 'function',
            name => 'acepta_conexion',
            virtualness => 'non_virtual',
            protection => 'public',
            static => 'no',
            brief => {},
            detailed => {},
            type => 'int',
            const => 'no',
            volatile => 'no',
            parameters => [
              {
                declaration_name => 'IDsocket',
                type => 'int'
              }
            ]
          }
        ]
      },
      brief => {},
      detailed => {}
    }
  ],
  groups => [
  ],
  pages => [
    {
      name => 'daemonizar',
      title4 => 'daemonizar',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Funcion que permite que un proceso se ejecute en segundo plano.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion permite a un proceso estar ejecutandose en segundo plano.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'sig'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'la sennal.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => '0 si se ejecuta sin ningun problema o -1 en caso de error.'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'liberaLista',
      title4 => 'liberaLista',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Libera la memoria de una lista.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Libera una lista que ha sido reservada.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'lista'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'lista que se quiere liberar. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'nElems'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'numero de elementos que componen la lista.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'funcionNick',
      title4 => 'funcionNick',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Realiza toda la funcionalidad correspondiente al nick de un usuario.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion se encarga de modificar el nick de un usuario, modificando el campo nick_name por el campo nick. Se modifica y este seria el nuevo nick del usuario actual.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'id'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'identificador del usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'usuario'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'nombre del usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'nick_name'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'nick actual del usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'real'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'realname del usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'nick'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'nick nuevo. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'IDsocket'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'el numero del identificador del socket.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'funcionUser',
      title4 => 'funcionUser',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Realiza toda la funcionalidad correspondiente a crear un nuevo usuario.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Imprime toda la informacion al conectarse un nuevo usuario.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'user'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'nombre del nuevo usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'nick'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'nick del nuevo usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'realname'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'realname del nuevo usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'modehost'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'IDsocket'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'el numero del identificador del socket.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'funcionPing',
      title4 => 'funcionPing',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Realiza toda la funcionalidad correspondiente con el comando ping.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion se encarga de cuando se realiza el comando ping, devuelva un pong. Este mensaje se mostraria cada 30 segundos.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'server1'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'Servidor 1. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'server2'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'servidor 2. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'IDsocket'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'el numero del identificador del socket.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'funcionAway',
      title4 => 'funcionAway',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Realiza toda la funcionalidad correspondiente con el comando away.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion se encarga de dejar ausente a un usuario.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'id'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'identificador del usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'usuario'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'nombre del usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'nick_name'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'nick actual del usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'real'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'realname del usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'away'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'away del usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'msg'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'mensaje que se quiere mostrar. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'IDsocket'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'el numero del identificador del socket.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'funcionPart',
      title4 => 'funcionPart',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Realiza toda la funcionalidad correspondiente a abandonar un canal.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion se encarga de que un usuario abandone un canal y en el caso de que no queden mas usuarios eliminar el canal.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'channel'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'canal del que se quiere salir el usuario. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'nick_name'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'nick del usuario que quiere abandonar el canal. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'msg'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'mensaje que se va a mandar. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'IDsocket'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'el numero del identificador del socket.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'funcionList',
      title4 => 'funcionList',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Realiza toda la funcionalidad correspondiente a listar los canales.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion se encarga de listar los canales creados.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'nick'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'nick del usuario actual. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'target'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'mascara. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'IDsocket'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'el numero del identificador del socket.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'funcionMotd',
      title4 => 'funcionMotd',
      detailed => {
        doc => [
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Realiza toda la funcionalidad correspondiente al mensaje del dia.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion se encarga de abrir un fichero y mostrar por pantalla el mensaje del dia.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'nick_name'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'nick del usuario actual. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'target'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'IDsocket'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'el numero del identificador del socket. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'fich'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'el nombre del fichero que se va a leer.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'manejador_alarma',
      title4 => 'manejador_alarma',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Funcion que se llama cada 30 segundos para que muestre el mensaje de PING.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion se llama cada 30 segundos con una alarma para mostrar el mensaje de PING.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'sig'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'la sennal.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'procesar',
      title4 => 'procesar',
      detailed => {
        doc => [
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Funcion que recibe los comandos del cliente.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion recibe y separa los comandos que manda el cliente.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => '*entrada'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'los comandos enviados por el cliente. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'IDsocket'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'numero del socket. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'readset'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'la lectura.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'servidor',
      title4 => 'servidor',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Funcion que se encarga de iniciar el servidor.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion realiza la apertura del socket y permite atender las distintas peticiones de los clientes.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'puerto'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'numero del puerto. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'path'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'direccion del fichero del mensaje del dia.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    },
    {
      name => 'parseCommand',
      title4 => 'parseCommand',
      detailed => {
        doc => [
          {
            type => 'text',
            content => 'Funcion en la que dependiendo del comando realiza una accion u otra.'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'parbreak'
          },
          {
            type => 'text',
            content => 'Esta funcion recibe un identificador que dependiendo de cual sea, permitira realizar una accion determinada.'
          },
          {
            type => 'parbreak'
          },
          params => [
            {
              parameters => [
                {
                  name => 'query'
                }
              ],
              doc => [
                {
                  type => 'text',
                  content => 'identificador del comando. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'comando'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'cadena a procesar. '
                }
              ]
            },
            {
              parameters => [
                {
                  name => 'IDsocket'
                }
              ],
              doc => [
                {
                  type => 'parbreak'
                },
                {
                  type => 'text',
                  content => 'identificador del socket.'
                }
              ]
            }
          ],
          {
            return => [
              {
                type => 'text',
                content => 'No devuelve nada'
              }
            ]
          },
          {
            author => [
              {
                type => 'text',
                content => 'Celia Mateos de Miguel ('
              },
              {
                type => 'url',
                content => 'cel.mateos@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Beatriz de Pablo Garcia ('
              },
              {
                type => 'url',
                content => 'beatriz.depablo@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ') Alfonso Sebares Mecha ('
              },
              {
                type => 'url',
                content => 'alfonso.sebares@estudiante.uam.es'
              },
              {
                type => 'text',
                content => ')'
              }
            ]
          },
          {
            date => [
              {
                type => 'text',
                content => '13 de febrero de 2017'
              }
            ]
          },
          {
            type => 'hruler'
          },
          {
            type => 'text',
            content => ' '
          }
        ]
      }
    }
  ]
};
1;
