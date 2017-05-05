0a1
> 
269c270
< 					char **lista = NULL, **listaUsuarios = NULL;
---
> 					char **lista = NULL, **listaNicksOnChannel = NULL, **listaUsuarios = NULL;
309a311
> 					listaNicksOnChannel = listaUsuarios;
313c315,324
< 					IRC_BuildStringsFromList(&uCanales, 512, ' ', &nstrings, listaUsuarios, nUsuarios);
---
> 					for(i = 0; i < nUsuarios; i++){
> 						/*syslog(LOG_INFO,"EN USUARIOS for %s",listaUsuarios[i]);*/
> 						if((IRCTAD_GetUserModeOnChannel (channel, listaUsuarios[i]) & IRCUMODE_OPERATOR)==IRCUMODE_OPERATOR)
> 							sprintf(listaNicksOnChannel[i],"@%s",listaUsuarios[i]);
> 						else
> 							sprintf(listaNicksOnChannel[i],"%s",listaUsuarios[i]);
> 					}
> 
> 
> 					IRC_BuildStringsFromList(&uCanales, 512, ' ', &nstrings, listaNicksOnChannel, nUsuarios);
384c395
< 				/*syslog(LOG_INFO, "Numero de canales %ld", numberOfChannels);*/
---
> 				syslog(LOG_INFO, "Numero de canales %ld", numberOfChannels);
447c458
< 				char **listaUsuarios = NULL, **uCanales = NULL;
---
> 				char **listaUsuarios = NULL, **listaNicksOnChannel = NULL, **uCanales = NULL;
459c470
< 				/*syslog(LOG_INFO, "NUMERO DE USUARIOS %ld", nUsuarios);*/
---
> 				syslog(LOG_INFO, "NUMERO DE USUARIOS %ld", nUsuarios);
460a472
> 					listaNicksOnChannel = listaUsuarios;
463c475,483
< 					IRC_BuildStringsFromList(&uCanales, 512, ' ', &nstrings, listaUsuarios, nUsuarios);
---
> 					for(i = 0; i < nUsuarios; i++){
> 						if((IRCTAD_GetUserModeOnChannel (channel, listaUsuarios[i]) & IRCUMODE_OPERATOR)==IRCUMODE_OPERATOR)
> 							sprintf(listaNicksOnChannel[i],"@%s",listaUsuarios[i]);
> 						else
> 							sprintf(listaNicksOnChannel[i],"%s",listaUsuarios[i]);
> 					}
> 
> 
> 					IRC_BuildStringsFromList(&uCanales, 512, ' ', &nstrings, listaNicksOnChannel, nUsuarios);
765a786
> 			printf("\nENTRA MODE Y NO SALGO\n");
774,776c795
< 			}else if(modo == NULL){
< 				syslog(LOG_INFO, "MODE: NULL.");
< 			}else {
---
> 			}else{
796c815
< 
---
> 					printf("\nCHECKPOINT\n");	
896c915
< }
\ No newline at end of file
---
> }
