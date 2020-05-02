#ifdef WIN32

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <Winsock2.h>
#include <stdlib.h>

#include "variable_declarations.c"
#include "procedures.c"

#else
#define closesocket close
#endif



int main(int argc, char **argv) {
    int ver1=(version/100);
    int ver2=(version/10)-(ver1*10);
    int ver3=(version-(ver1*100)-(ver2*10));
    printf("Running server version %d.%d%d\n", ver1, ver2, ver3);
    printf("Initializing variables...");

    //tu byly deklaracje zmiennych

    fd_set input;
//end of declarations;
    printf("done\n");
    printf("Enter addres: (d for default: %s)\n>", D_HOST2);
    scanf("%s",&D_HOST);
    if(D_HOST[0]==100)
        strcpy(D_HOST,D_HOST2);
    if(D_HOST[0]==99)
        strcpy(D_HOST,D_HOST3);

    printf("Loading quest data...\n");
    plik01 = fopen("data/quests.dat", "r" );
    while (feof(plik01)==0) {
        fscanf(plik01,"%d,",&j);
        fscanf(plik01,"%d,",&quest_lvl[j]);
        fscanf(plik01,"%10[^, \t\n\r]\n", quest_oddaj[j]);
        fscanf(plik01,"%d,%d,%d\n",&quest_typ[j],&quest_dozabicia[j],&quest_moby_rodzaje[j]);
        for(i=0; i<quest_moby_rodzaje[j]; i++) {
            if((i+1)!=quest_moby_rodzaje[j]) {
                fscanf(plik01,"%d,",&quest_moby[j][i]);
            }
            else {
                fscanf(plik01,"%d\n",&quest_moby[j][i]);
            }
        }
        for(i=0; i<quest_moby_rodzaje[j]; i++) {
            if((i+1)!=quest_moby_rodzaje[j]) {
                fscanf(plik01,"%d,",&quest_moby_ilosc[j][i]);
            }
            else {
                fscanf(plik01,"%d\n",&quest_moby_ilosc[j][i]);
            }
        }
    }
    fclose(plik01);

    printf("Loading item data...\n");
    plik01 = fopen("data/items.dat", "r" );
    while (feof(plik01)==0) {
        fscanf(plik01,"%d,",&j);
        fscanf(plik01,"%30[^, \t\n\r]\n", item_nazwa[j]);
        fscanf(plik01,"%d,%d,%d,%d,%d\n",&item_grafika[j],&item_typ[j],&item_wartosc[j],&item_dura[j],&item_stack[j]);
        fscanf(plik01,"%d,%d,%d,%d\n",&item_arm[j],&item_md[j],&item_bd[j],&item_bh[j]);
        fscanf(plik01,"%d,%d,%d,%d,%d\n",&item_int[j],&item_spi[j],&item_sta[j],&item_agi[j],&item_str[j]);
    }
    fclose(plik01);

    printf("Loading user data...\n");
    plik01 = fopen("login/login.dat", "r" );
    j=0;
    while (feof(plik01)==0) {
        fscanf(plik01,"%10[^, \t\n\r]:%10[^, \t\n\r]\n", login[j], pass[j]);
        printf("loading user %s:%s\n",login[j],pass[j]);
        strcpy(sciezka, "login/");
        strcat(sciezka, login[j]);
        strcat(sciezka, "/position");
        printf("opening file: %s\n",sciezka);
        plik02 = fopen(sciezka, "r" );
        fscanf(plik02,"%d,%d,%d\n",&konto_start[j][0],&konto_start[j][1],&konto_start[j][2]);
        fscanf(plik02,"%d,%d,%d,%d\n",&konto_lvl[j], &konto_exp[j], &konto_hp[j], &konto_mp[j]);
        fscanf(plik02,"%d\n",&konto_klasa[j]);
        fscanf(plik02,"%d\n",&konto_buff_index[j]);
        for(i=0; i<konto_buff_index[j]; i++)
            fscanf(plik02,"%d,%d,%d,%d,%d,%d\n",&konto_buff[j][i],&konto_buff_time[j][i],&konto_buff_freq[j][i],&konto_buff_interrupt[j][i],&konto_buff_attr1[j][i],&konto_buff_attr2[j][i]);
        wylicz_staty(konto_klasa[j],konto_lvl[j], -1);
        konto_hp_max[j]=w_stamina*14; //if(konto_hp[j]>konto_hp_max[j]) konto_hp[j]=konto_hp_max[j];
        konto_mp_max[j]=w_intelect*10; //if(konto_mp[j]>konto_mp_max[j]) konto_mp[j]=konto_mp_max[j];
        konto_spirit[j]=w_spirit;
        fclose(plik02);
        strcpy(sciezka, "login/");
        strcat(sciezka, login[j]);
        strcat(sciezka, "/inv");
        plik02 = fopen(sciezka, "r" );
        for(i=0; i<29; i++) {
            fscanf(plik02,"%d,",&konto_equip[j][i]);
        }
        i=29;
        fscanf(plik02,"%d\n",&konto_equip[j][i]);
        fscanf(plik02,"%d,%d,%d,%d\n",&konto_bagi[j][0],&konto_bagi[j][1],&konto_bagi[j][2],&konto_bagi[j][3]);
        for(i=0; i<5; i++) {
            for(k=0; k<14; k++)
                fscanf(plik02,"%d,",&konto_inventory[j][k+(i*15)]);
            k=14;
            fscanf(plik02,"%d\n",&konto_inventory[j][k+(i*15)]);
            for(k=0; k<14; k++)
                fscanf(plik02,"%d,",&konto_inventory_ilosc[j][k+(i*15)]);
            k=14;
            fscanf(plik02,"%d\n",&konto_inventory_ilosc[j][k+(i*15)]);
        }
        fscanf(plik02,"%d\n",&konto_kasa[j]);
        fclose(plik02);
        strcpy(sciezka, "login/");
        strcat(sciezka, login[j]);
        strcat(sciezka, "/quest");
        plik02 = fopen(sciezka, "r" );
        for(i=0; i<10; i++) {
            if(i!=9)
                fscanf(plik02,"%d,",&konto_quest[j][i]);
            else
                fscanf(plik02,"%d\n",&konto_quest[j][i]);
        }
        for(k=0; k<10; k++) {
            for(i=0; i<8; i++) {
                if(i!=7)
                    fscanf(plik02,"%d,",&konto_quest_finnish[j][k][i]);
                else
                    fscanf(plik02,"%d\n",&konto_quest_finnish[j][k][i]);
            }
        }
        fclose(plik02);
        j++;
    }
    ilosc_kont = j;

    fclose(plik01);

    printf("Initializing variables and loading data from files...\n");

    for(j=1; j<(il_map+1); j++) { //ladowanie NPC
        printf("\nLoading map %d...\n\n",j);
        strcpy(sciezka, "data/00000_npcs.dat");
        sciezka[9]= 48+j;  //zmienic zeby obslugiwalo >10 map
        plik01 = fopen( sciezka, "r" );
        nr_post=0;
        npc_max[j]=0;
        for (i=0; i<65536; i++)
            bytes_sent[i]=0;

        while (feof(plik01)==0) {
            fscanf(plik01,"%10[^, \t\n\r],", npc_nick[j][nr_post]);
            for(i=0; i<10; i++) { // zamiana "_" na " "
                if(npc_nick[j][nr_post][i]==95) {
                    npc_nick[j][nr_post][i]=32;
                }
            }
            //printf("NICK: %s ", npc_nick[j][nr_post]);
            fscanf(plik01,"%d,%d,%d\n", &npc_gfx[j][nr_post], &npc_type[j][nr_post], &npc_posn[j][nr_post]);
            //printf(" GFX: %d TYP: %d IL_R: %d\n", npc_gfx[j][nr_post], npc_type[j][nr_post], npc_posn[j][nr_post]);
            for (i=0; i<npc_posn[j][nr_post]; i++) {
                if((i+1)==(npc_posn[j][nr_post]))  //jesli to ostatnia pozycja
                    fscanf(plik01,"%d,%d,%d\n", &npc_pos[j][nr_post][i][0], &npc_pos[j][nr_post][i][1], &npc_speed[j][nr_post][i]);
                else    //jesli nie
                    fscanf(plik01,"%d,%d,%d, ", &npc_pos[j][nr_post][i][0], &npc_pos[j][nr_post][i][1], &npc_speed[j][nr_post][i]);
                npc_pos[j][nr_post][i][0]=npc_pos[j][nr_post][i][0]*klocki;
                npc_pos[j][nr_post][i][1]=npc_pos[j][nr_post][i][1]*klocki;
                //printf("%d,%d,%d  ", npc_pos[j][nr_post][i][0], npc_pos[j][nr_post][i][1],npc_speed[j][nr_post][i]);
            }
            npc_pos_x[j][nr_post]=npc_pos[j][nr_post][0][0];
            npc_pos_y[j][nr_post]=npc_pos[j][nr_post][0][1];
            npc_pos_d[j][nr_post]=0;
            npc_droga[j][nr_post]=1; //postac zaczyna na 0 punkcie, niech zmierza do 1.
            for(stan_i=0; stan_i<D_SOCKETS; stan_i++)
                npc_stan[j][nr_post][stan_i]=0;
            npc_max[j]++;
            nr_post++;
            printf("Loaded char no: %d EOF: %d\n",nr_post,feof(plik01));
            Sleep(1);
        }
        npc_max[j]=nr_post; //ilosc postaci na danej mapie
        fclose(plik01);
    }

    printf("Loading mobs.dat...\n");
    plik01 = fopen("data/mobs.dat", "r" );
    mob_data_max=0;
    while (feof(plik01)==0) {
        fscanf(plik01,"%d\n", &mob_data_index);
        fscanf(plik01,"%10[^, \t\n\r],", mob_data_nick[mob_data_index]);
        for(i=0; i<10; i++) { // zamiana "_" na " "
            if(mob_data_nick[mob_data_index][i]==95) {
                mob_data_nick[mob_data_index][i]=32;
            }
        }
        fscanf(plik01,"%d,%d,%d,%d,%d\n", &mob_data_gfx[mob_data_index], &mob_data_type[mob_data_index], &mob_data_cz_atak[mob_data_index], &mob_data_moc_atak1[mob_data_index], &mob_data_moc_atak2[mob_data_index]);
        fscanf(plik01,"%d,%d,%d,%d\n", &mob_data_lvl[mob_data_index], &mob_data_max_hp[mob_data_index], &mob_data_max_mp[mob_data_index], &mob_data_loot_il[mob_data_index]);
        for (i=0; i<mob_data_loot_il[mob_data_index]; i++) {
            if((i+1)==(mob_data_loot_il[mob_data_index]))  //jesli to ostatnia pozycja
                fscanf(plik01,"%d,%d\n", &mob_data_loot_index[mob_data_index][i], &mob_data_loot_szansa[mob_data_index][i]);
            else    //jesli nie
                fscanf(plik01,"%d,%d, ", &mob_data_loot_index[mob_data_index][i], &mob_data_loot_szansa[mob_data_index][i]);
        }
        mob_data_max++;
        printf("mobs.dat - Loaded char no: %d EOF: %d\n",mob_data_max,feof(plik01));
        Sleep(1);
    }
    fclose(plik01);

    for(j=1; j<(il_map+1); j++) { //ladowanie mobkow
        printf("\nLoading map %d...\n\n",j);
        strcpy(sciezka, "data/00000_mobs.dat");
        sciezka[9]= 48+j;  //zmienic zeby obslugiwalo >10 map
        plik01 = fopen( sciezka, "r" );
        nr_post=0;
        mob_max[j]=0;
        for (i=0; i<65536; i++)
            bytes_sent[i]=0;
        while (feof(plik01)==0) {
            //fscanf(plik01,"%10[^, \t\n\r],", mob_nick[j][nr_post]);
            fscanf(plik01,"%d,", &mob_index[j][nr_post]);
            mob_data_index = mob_index[j][nr_post];
            for(i=0; i<10; i++) { //skopiuj nick
                mob_nick[j][nr_post][i]=mob_data_nick[mob_data_index][i];
            }
            printf("NICK: %s ", mob_nick[j][nr_post]);
            mob_gfx[j][nr_post]=mob_data_gfx[mob_data_index];
            mob_type[j][nr_post]=mob_data_type[mob_data_index];
            mob_cz_atak[j][nr_post]=mob_data_cz_atak[mob_data_index];
            mob_moc_atak1[j][nr_post]=mob_data_moc_atak1[mob_data_index];
            mob_moc_atak2[j][nr_post]=mob_data_moc_atak2[mob_data_index];
            fscanf(plik01,"%d\n", &mob_posn[j][nr_post]);
            printf(" GFX: %d TYP: %d IL_R: %d\n", mob_gfx[j][nr_post], mob_type[j][nr_post], mob_posn[j][nr_post]);
            for (i=0; i<mob_posn[j][nr_post]; i++) {
                if((i+1)==(mob_posn[j][nr_post]))  //jesli to ostatnia pozycja
                    fscanf(plik01,"%d,%d,%d\n", &mob_pos[j][nr_post][i][0], &mob_pos[j][nr_post][i][1], &mob_speed[j][nr_post][i]);
                else    //jesli nie
                    fscanf(plik01,"%d,%d,%d, ", &mob_pos[j][nr_post][i][0], &mob_pos[j][nr_post][i][1], &mob_speed[j][nr_post][i]);
                mob_pos[j][nr_post][i][0]=mob_pos[j][nr_post][i][0]*klocki;
                mob_pos[j][nr_post][i][1]=mob_pos[j][nr_post][i][1]*klocki;
                printf("%d,%d,%d  ", mob_pos[j][nr_post][i][0], mob_pos[j][nr_post][i][1],mob_speed[j][nr_post][i]);
            }
            mob_loot_il[j][nr_post]=mob_data_loot_il[mob_data_index];
            for(i=0; i<mob_loot_il[j][nr_post]; i++) {
                mob_loot_index[j][nr_post][i]=mob_data_loot_index[mob_data_index][i];
                mob_loot_szansa[j][nr_post][i]=mob_data_loot_szansa[mob_data_index][i];
            }
            mob_lvl[j][nr_post]=mob_data_lvl[mob_data_index];
            mob_max_hp[j][nr_post]=mob_data_max_hp[mob_data_index];
            mob_max_mp[j][nr_post]=mob_data_max_mp[mob_data_index];
            printf("\nLVL: %d HP: %d MP:%d ", mob_lvl[j][nr_post], mob_max_hp[j][nr_post],mob_max_mp[j][nr_post]);
            mob_hp[j][nr_post]=mob_max_hp[j][nr_post];
            mob_mp[j][nr_post]=mob_max_mp[j][nr_post];
            mob_pos_x[j][nr_post]=mob_pos[j][nr_post][0][0];
            mob_pos_y[j][nr_post]=mob_pos[j][nr_post][0][1];
            mob_pos_d[j][nr_post]=0;
            mob_target[j][nr_post]=-1;
            mob_atak[j][nr_post]=0;
            mob_droga[j][nr_post]=1; //postac zaczyna na 0 punkcie, niech zmierza do 1.
            for(stan_i=0; stan_i<D_SOCKETS; stan_i++)
                mob_stan[j][nr_post][stan_i]=0;
            mob_max[j]++;
            nr_post++;
            printf("Loaded char no: %d EOF: %d\n",nr_post,feof(plik01));
            Sleep(1);
        }
        mob_max[j]=nr_post; //ilosc postaci na danej mapie
        fclose(plik01);
    }



    /* read the delay if any */
    if (argc > 1)
        delay = atol(argv[1]);
    else
        delay = 0;

#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif /* WIN32 */

    /* create a socket */
    descriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (descriptor == -1) {
        perror("socket");
        return (1);
    }

    /* get information about the host */
    memset(&addr, 0, sizeof(addr));
    host = gethostbyname(D_HOST);
    if (host == NULL) {
        perror("gethostbyname");
        closesocket(descriptor);
#ifdef WIN32
        WSACleanup();
#endif
        return (1);
    }

    /* bind the socket to an address and port */
    memcpy(&addr.sin_addr, host->h_addr_list[0], sizeof(host->h_addr_list[0]));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(D_PORT);
    result = bind(descriptor, (struct sockaddr *)&addr, sizeof(addr));
    if (result == -1) {
        perror("bind");
        closesocket(descriptor);
#ifdef WIN32
        WSACleanup();
#endif
        return (1);
    }

    /* listen for connections */
    result = listen(descriptor, D_QUEUE);
    if (result == -1) {
        perror("listen");
        closesocket(descriptor);
#ifdef WIN32
        WSACleanup();
#endif
        return (1);
    }

    memset(sockets, 0, sizeof(sockets));
    maximun = descriptor;



    endwait = getTime () + 250 ; //pierwsza klatka
    //endwait = time(NULL)+1; //pierwsza klatka

    result = 0;
//		while (result != -1) {
    while (result != -2) {
        FD_ZERO(&input);
        FD_SET(descriptor, &input);
        for (result = 0; result < sockets_index; result++)
            FD_SET(sockets[result], &input);
        for (result = 0; result < sockets_waiting_index; result++)
            FD_SET(sockets_waiting[result], &input);


        tv.tv_sec = delay;
        tv.tv_usec = 0;
        if (delay == -1)
            result = select(maximun + 1, &input, NULL, NULL, NULL);
        else
            result = select(maximun + 1, &input, NULL, NULL, &tv);

        for (index = 0; index < sockets_index; index++) {  //petla w ktorej sa zmiany w sprawie graczy
            if (odpowiedz[sockets[index]]<(0-80)) {	//wywalanie zlaggowanych zalogowanych uzytkownikow
                //WYLOGOWANIE KLIENTA
                //zapisywanie pozycji
                konto_start[nr_konta[sockets[index]]][0]=player_map[(sockets[index])];
                konto_start[nr_konta[sockets[index]]][1]=player_x[(sockets[index])];
                konto_start[nr_konta[sockets[index]]][2]=player_y[(sockets[index])];
                konto_hp[nr_konta[sockets[index]]]=player_hp[(sockets[index])];
                konto_mp[nr_konta[sockets[index]]]=player_mp[(sockets[index])];
                konto_hp_max[nr_konta[sockets[index]]]=player_hp_max[(sockets[index])];
                konto_mp_max[nr_konta[sockets[index]]]=player_mp_max[(sockets[index])];
                konto_exp[nr_konta[sockets[index]]]=player_exp[(sockets[index])];
                konto_lvl[nr_konta[sockets[index]]]=player_lvl[(sockets[index])];
                konto_klasa[nr_konta[sockets[index]]]=player_klasa[(sockets[index])];
                konto_kasa[nr_konta[sockets[index]]]=player_kasa[(sockets[index])];
                for(stan_i=0; stan_i<8; stan_i++)	konto_quest[nr_konta[sockets[index]]][stan_i]=player_quest[nr_konta[sockets[index]]][stan_i];
                for(stan_m=0; stan_m<10; stan_m++) {
                    for(stan_i=0; stan_i<8; stan_i++) {
                        konto_quest_finnish[nr_konta[sockets[index]]][stan_m][stan_i]=player_quest_finnish[nr_konta[sockets[index]]][stan_m][stan_i];
                    }
                }
                for(stan_i=0; stan_i<30; stan_i++)
                    konto_equip[nr_konta[sockets[index]]][stan_i]=player_equip[(sockets[index])][stan_i];
                for(stan_i=0; stan_i<5; stan_i++)
                    konto_bagi[nr_konta[sockets[index]]][stan_i]=player_bagi[(sockets[index])][stan_i];
                for(stan_i=0; stan_i<75; stan_i++) {
                    konto_inventory[nr_konta[sockets[index]]][stan_i]=player_inventory[(sockets[index])][stan_i];
                    konto_inventory_ilosc[nr_konta[sockets[index]]][stan_i]=player_inventory_ilosc[(sockets[index])][stan_i];
                }
                konto_buff_index[nr_konta[sockets[index]]]=player_buff_index[(sockets[index])];
                for(stan_i=0; stan_i<16; stan_i++) {
                    konto_buff[nr_konta[sockets[index]]][stan_i]=player_buff[(sockets[index])][stan_i];
                    konto_buff_time[nr_konta[sockets[index]]][stan_i]=player_buff_time[(sockets[index])][stan_i]-cycle;
                    konto_buff_freq[nr_konta[sockets[index]]][stan_i]=player_buff_freq[(sockets[index])][stan_i];
                    konto_buff_interrupt[nr_konta[sockets[index]]][stan_i]=player_buff_interrupt[(sockets[index])][stan_i];
                    konto_buff_attr1[nr_konta[sockets[index]]][stan_i]=player_buff_attr1[(sockets[index])][stan_i];
                    konto_buff_attr2[nr_konta[sockets[index]]][stan_i]=player_buff_attr2[(sockets[index])][stan_i];
                }
                strcpy(sciezka, "login/");
                strcat(sciezka, login[nr_konta[sockets[index]]]);
                strcat(sciezka, "/position");
                printf("writing to file: %s\n",sciezka);
                plik02 = fopen(sciezka, "w" );
                fprintf(plik02,"%d,%d,%d\n", player_map[(sockets[index])], player_x[(sockets[index])], player_y[(sockets[index])]);
                fprintf(plik02,"%d,%d,%d,%d\n",player_lvl[(sockets[index])], player_exp[(sockets[index])], player_hp[(sockets[index])], player_mp[(sockets[index])]);
                fprintf(plik02,"%d\n",player_klasa[(sockets[index])]);
                fprintf(plik02,"%d\n",player_buff_index[(sockets[index])]);
                for(i=0; i<player_buff_index[sockets[index]]; i++)
                    fprintf(plik02,"%d,%d,%d,%d,%d,%d\n",player_buff[sockets[index]][i],(player_buff_time[sockets[index]][i]-cycle),player_buff_freq[sockets[index]][i],player_buff_interrupt[sockets[index]][i],player_buff_attr1[sockets[index]][i],player_buff_attr2[sockets[index]][i]);
                fclose(plik02);
                strcpy(sciezka, "login/");
                strcat(sciezka, login[nr_konta[sockets[index]]]);
                strcat(sciezka, "/inv");
                printf("writing to file: %s\n",sciezka);
                plik02 = fopen(sciezka, "w" );
                for(stan_i=0; stan_i<29; stan_i++)
                    fprintf(plik02,"%d,",player_equip[(sockets[index])][stan_i]);
                stan_i=29;
                fprintf(plik02,"%d\n",player_equip[(sockets[index])][stan_i]);
                fprintf(plik02,"%d,%d,%d,%d\n", player_bagi[(sockets[index])][0],player_bagi[(sockets[index])][1],player_bagi[(sockets[index])][2],player_bagi[(sockets[index])][3]);
                for(stan_m=0; stan_m<5; stan_m++) {
                    for(stan_i=0; stan_i<14; stan_i++)
                        fprintf(plik02,"%d,",player_inventory[(sockets[index])][stan_i+(stan_m*15)]);
                    stan_i=14;
                    fprintf(plik02,"%d\n",player_inventory[(sockets[index])][stan_i+(stan_m*15)]);
                    for(stan_i=0; stan_i<14; stan_i++)
                        fprintf(plik02,"%d,",player_inventory_ilosc[(sockets[index])][stan_i+(stan_m*15)]);
                    stan_i=14;
                    fprintf(plik02,"%d\n",player_inventory_ilosc[(sockets[index])][stan_i+(stan_m*15)]);
                }
                fprintf(plik02,"%d\n",player_kasa[(sockets[index])]);
                fclose(plik02);
                strcpy(sciezka, "login/");
                strcat(sciezka, login[nr_konta[sockets[index]]]);
                strcat(sciezka, "/quest");
                printf("writing to file: %s\n",sciezka);
                plik02 = fopen(sciezka, "w" );
                for(stan_i=0; stan_i<9; stan_i++)	fprintf(plik02,"%d,",player_quest[(sockets[index])][stan_i]);
                fprintf(plik02,"%d\n",player_quest[(sockets[index])][9]);
                for(stan_m=0; stan_m<10; stan_m++) {
                    for(stan_i=0; stan_i<8; stan_i++) {
                        if(stan_i!=7)
                            fprintf(plik02,"%d,",player_quest_finnish[(sockets[index])][stan_m][stan_i]);
                        else
                            fprintf(plik02,"%d\n",player_quest_finnish[(sockets[index])][stan_m][stan_i]);
                    }
                }
                fclose(plik02);
                //koniec zapisywania pozycji

                odpowiedz[(sockets[index])]=0;
                buffer2[0]= 80;  //P - postac
                tymczasowa=(sockets[index]/128);
                buffer2[1]= 128+32+tymczasowa;
                buffer2[2]= 128+sockets[index]-(tymczasowa*128);
                buffer2[3]= 68;  //R - ruch Q - nick D68 - usuniecie
                player_x[(sockets[index])]=-1;
                player_y[(sockets[index])]=-1;
                buffer2[4]= 128;
                buffer2[5]= 127;
                buffer2[6]= 128;
                buffer2[7]= 127;
                buffer2[8]= 128+1;
                buffer2[9]= 128;
                for(n=0; n<10; n++)
                    buffer2[10+n]= 32;
                buffer2[20]= 13;
                buffer2[21]= 10;
                result_s = -666;
                for (j=0; j<sockets_index; j++) {
                    if(sockets[j]!=sockets[index]) {
                        result_s = send(sockets[j], buffer2, 22, 0);
                        bytes_sent[(sockets[j])] += 22;
                    }
                }
                for(stan_m=1; stan_m<(il_map+1); stan_m++) {
                    for(stan_i=0; stan_i<mob_max[stan_m]; stan_i++) {
                        if(mob_target[stan_m][stan_i]==sockets[index])
                            mob_target[stan_m][stan_i]=-1;
                    }
                }
                sockets_index--;
                bytes_sent[(sockets[index])]=0;
                for(i=index; i<sockets_index; i++) {
                    sockets[i]=sockets[i+1];
                    for(stan_m=1; stan_m<(il_map+1); stan_m++) { //wszystko co sie rusza
                        for(stan_i=0; stan_i<npc_max[stan_m]; stan_i++)
                            npc_stan[stan_m][stan_i][i]=npc_stan[stan_m][stan_i][i+1];
                        for(stan_i=0; stan_i<mob_max[stan_m]; stan_i++)
                            mob_stan[stan_m][stan_i][i]=mob_stan[stan_m][stan_i][i+1];
                    }
                    for(stan_i=0; stan_i<sockets_index; stan_i++)
                        player_stan[sockets[stan_i]][i]=player_stan[sockets[stan_i]][i+1];
                }
                //KONIEC WYLOGOWYWANIA
            }
            int wylicz = cycle/4;  //co 8 sekund
            if( (cycle==(wylicz*4)) && (regen[sockets[index]]==0) && (player_hp[sockets[index]]>0) ) {             // REGEN
                if(player_klasa[sockets[index]]<16) {  //z mana
                    if(player_czas_cast[sockets[index]]==0) {
                        player_hp[sockets[index]]+=(player_spirit[sockets[index]]/16);
                        player_mp[sockets[index]]+=((player_spirit[sockets[index]]*5)/16);
                        player_info[sockets[index]]=1;
                        player_stan[sockets[index]][index]=0;
                    }
                    //printf("spirit: %d\n", player_spirit[sockets[index]]);

                }
                else { //tu bez many
                    player_hp[sockets[index]]+=((player_spirit[sockets[index]])*5)/4;
                    player_info[sockets[index]]=1;
                    player_stan[sockets[index]][index]=0;
                }
                if(player_hp[sockets[index]]>player_hp_max[sockets[index]])	player_hp[sockets[index]]=player_hp_max[sockets[index]];
                if(player_mp[sockets[index]]>player_mp_max[sockets[index]])	player_mp[sockets[index]]=player_mp_max[sockets[index]];

                regen[sockets[index]]=1;
            }
            if (cycle!=(wylicz*4))
                regen[sockets[index]]=0;
            if(player_hp[sockets[index]]<1) //wygas spelle jesli gracz padl
                spelle_l[sockets[index]]=0;
            if((player_buff_index[sockets[index]]>0) && (cycle_buffy[sockets[index]]!=cycle)) { // BUFFY GRACZY
                cycle_buffy[sockets[index]]=cycle;
                for(i=0; i<player_buff_index[sockets[index]]; i++) {
                    if( ((cycle>player_buff_time[sockets[index]][i])&&(player_buff_time[sockets[index]][i]>0)) || ((player_ruch[sockets[index]]>0) && (player_buff_interrupt[sockets[index]][i]==1)) ) {
                        //buff sie skonczyl - usun go (lub gracz dokonal ruchu)								buffer2[0]=66; //B (66) - informacje o buffie
                        buffer2[0]= 66;  //B - info o buffie
                        tymczasowa = player_buff[sockets[index]][i]/128;
                        buffer2[1]= 128+tymczasowa;
                        buffer2[2]= 128+(player_buff[sockets[index]][i])-(tymczasowa*128);
                        buffer2[3]= 128;
                        buffer2[4]= 128;
                        buffer2[5]= 128;  //128 - kasowanie
                        buffer2[6]= 13;
                        buffer2[7]= 10;
                        result_s = send(sockets[index], buffer2, 8, 0);
                        for(j=i; j<(player_buff_index[sockets[index]]); j++) {
                            player_buff[sockets[index]][j]=player_buff[sockets[index]][j+1];
                            player_buff_attr1[sockets[index]][j]=player_buff_attr1[sockets[index]][j+1];
                            player_buff_attr2[sockets[index]][j]=player_buff_attr2[sockets[index]][j+1];
                            player_buff_time[sockets[index]][j]=player_buff_time[sockets[index]][j+1];
                            player_buff_freq[sockets[index]][j]=player_buff_freq[sockets[index]][j+1];
                            player_buff_interrupt[sockets[index]][j]=player_buff_interrupt[sockets[index]][j+1];
                            player_buff_index[sockets[index]]--;
                            printf("Kasuje buffa!\n");
                        }
                        i--;
                        if(i<0) i=0;
                    }
                    //if((player_ruch[sockets[index]]>0) && (player_buff_interrupt[sockets[index]][i]==1)) {
                    //}
                }
                for(i=0; i<player_buff_index[sockets[index]]; i++) {
                    int cycle_tymcz=cycle/player_buff_freq[sockets[index]][i];
                    if(cycle==cycle_tymcz*player_buff_freq[sockets[index]][i]) {
                        switch(player_buff[sockets[index]][i]) {
                        case 1: //regen
                            player_hp[sockets[index]]+=player_buff_attr1[sockets[index]][i]*10;
                            player_mp[sockets[index]]+=player_buff_attr2[sockets[index]][i]*10;
                            if(player_hp[sockets[index]]>player_hp_max[sockets[index]])	player_hp[sockets[index]]=player_hp_max[sockets[index]];
                            if(player_mp[sockets[index]]>player_mp_max[sockets[index]])	player_mp[sockets[index]]=player_mp_max[sockets[index]];
                            break;
                        }
                    }
                }
            }
            if(spelle_l[sockets[index]]>0) { 		// SPELLE  GRACZY
                int koszt_spella_kopia = koszt_spella(spelle[sockets[index]][0], ((player_lvl[sockets[index]]+1)/2));
                int od_x=(player_x[sockets[index]]*klocki)-mob_pos_x[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)];
                int od_y=(player_y[sockets[index]]*klocki)-mob_pos_y[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)];
                int od_l=zasieg_spella(spelle[sockets[index]][0])*klocki;
                //printf("X: %d Y: %d L: %d - wynik: %d\n",od_x,od_y,od_l,odleglosc(od_x,od_y,od_l));
                if( (koszt_spella_kopia<player_mp[sockets[index]]) ) {  //jesli gracz ma tyle many zeby castnac i czy jest w zasiegu
                    if(spelle_endwait[sockets[index]]<getTime()) {	//jesli spell skonczyl sie castowac
                        //buffer2[0]=65; //A (65) - atak, informacja o cascie
                        //buffer2[1]=128+3; // 0 - rozpoczecie 1 - cofniecie 2- przerwanie 3-zakonczenie
                        //buffer2[2]= 13;	buffer2[3]= 10;
                        //result_s = send(sockets[index], buffer2, 4, 0);

                        int rodzaj_postaci=spelle_mob[sockets[index]][0]/4096;
                        int rodzaj_spella = spelle[sockets[index]][0];
                        if(rodzaj_spella<21) rodzaj_spella=1;  //wszystkie spelle 1-20 to spelle tego samego typu
                        switch(spelle[sockets[index]][0]) {
                        case 1:
                            if(rodzaj_postaci==2) {
                                if(((mob_hp[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)])>0) && (odleglosc(od_x,od_y,od_l)==1))  {  //jesli mob zyje i nie jest za daleko
                                    mob_target[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)]=sockets[index];
                                    player_czas_cast[sockets[index]]=(4*6)/cycleX; 		//6 sekund bez casta to pelny regen
                                    player_mp[sockets[index]]-=koszt_spella_kopia;
                                    for(stan_i=0; stan_i<sockets_index; stan_i++)
                                        player_stan[sockets[index]][stan_i]=0;
                                    int hit=wylicz_hit(spelle[sockets[index]][0], ((player_lvl[sockets[index]]+1)/2));
                                    mob_hp[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)]-=hit;
                                    for(stan_i=0; stan_i<sockets_index; stan_i++)
                                        mob_stan[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)][stan_i]=0;
                                    zysk_expa = 0;
                                    if(mob_hp[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)]<0) {  //zabicie mobka
                                        //mob_target[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)]=-1;
                                        mob_hp[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)]=0;
                                        zysk_expa=killexp(mob_lvl[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)], player_lvl[sockets[index]]);
                                        player_exp[sockets[index]]+=zysk_expa;
                                        if(player_exp[sockets[index]]>exp(player_lvl[sockets[index]])) {
                                            player_exp[sockets[index]]-=exp(player_lvl[sockets[index]]);
                                            player_lvl[sockets[index]]++;
                                            wylicz_staty(player_klasa[sockets[index]],player_lvl[sockets[index]],sockets[index]);
                                            player_hp_max[sockets[index]]=w_stamina*14;
                                            player_mp_max[sockets[index]]=w_intelect*10;
                                            player_spirit[sockets[index]]=w_spirit;
                                        }
                                        player_info[sockets[index]]=1;
                                        player_stan[sockets[index]][index]=0;
                                        for(stan_i=0; stan_i<10; stan_i++) {
                                            if(player_quest[sockets[index]][stan_i]>0) {
                                                if(quest_typ[player_quest[sockets[index]][stan_i]]==1) {

                                                    int temp_dozabicia=0;
                                                    if(quest_dozabicia[player_quest[sockets[index]][stan_i]]==0) {
                                                        printf("!!!\n"); //jesli mobki podlicza szczegolowo
                                                        for(stan_m=0; stan_m<8; stan_m++) {
                                                            if(quest_moby[player_quest[sockets[index]][stan_i]][stan_m]==mob_index[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)]) {

                                                                if( (player_quest_finnish[sockets[index]][stan_i][stan_m]<quest_moby_ilosc[player_quest[sockets[index]][stan_i]][stan_m]) && (temp_dozabicia==0) ) {
                                                                    //temp_dozabicia=1;
                                                                    player_quest_finnish[sockets[index]][stan_i][stan_m]++;
                                                                    printf("quest: %d   moby nr: %d   ilosc %d/%d\n",player_quest[sockets[index]][stan_i],quest_moby[player_quest[sockets[index]][stan_i]][stan_m],player_quest_finnish[sockets[index]][stan_i][stan_m],quest_moby_ilosc[player_quest[sockets[index]][stan_i]][stan_m]);
                                                                    //do_zabicia=quest_dozabicia[player_quest[sockets[index]][stan_i]];

                                                                    tymczasowa = stan_i;
                                                                    buffer2[1]= 128+tymczasowa;   		//numer slotu na questa u gracza (1-10)
                                                                    tymczasowa = ((player_quest[sockets[index]][stan_i])/(128*128));
                                                                    tymczasowa2 = ((player_quest[sockets[index]][stan_i])/128)-(tymczasowa*128);
                                                                    buffer2[2]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[3]= 128+(tymczasowa2)-tymczasowa*128;
                                                                    buffer2[4]= 128+(player_quest[sockets[index]][stan_i])-(tymczasowa2*128)-(tymczasowa*128*128);
                                                                    tymczasowa = ((quest_lvl[player_quest[sockets[index]][stan_i]])/128);
                                                                    buffer2[5]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[6]= 128+(quest_lvl[player_quest[sockets[index]][stan_i]])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_typ[player_quest[sockets[index]][stan_i]])/128);
                                                                    buffer2[7]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[8]= 128+(quest_typ[player_quest[sockets[index]][stan_i]])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_dozabicia[player_quest[sockets[index]][stan_i]])/128);
                                                                    buffer2[9]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[10]= 128+(quest_dozabicia[player_quest[sockets[index]][stan_i]])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_moby_rodzaje[player_quest[sockets[index]][stan_i]])/128);
                                                                    buffer2[11]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[12]= 128+(quest_moby_rodzaje[player_quest[sockets[index]][stan_i]])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_moby[player_quest[sockets[index]][stan_i]][0])/128);
                                                                    buffer2[13]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[14]= 128+(quest_moby[player_quest[sockets[index]][stan_i]][0])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_moby[player_quest[sockets[index]][stan_i]][1])/128);
                                                                    buffer2[15]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[16]= 128+(quest_moby[player_quest[sockets[index]][stan_i]][1])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_moby[player_quest[sockets[index]][stan_i]][2])/128);
                                                                    buffer2[17]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[18]= 128+(quest_moby[player_quest[sockets[index]][stan_i]][2])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_moby[player_quest[sockets[index]][stan_i]][3])/128);
                                                                    buffer2[19]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[20]= 128+(quest_moby[player_quest[sockets[index]][stan_i]][3])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_moby[player_quest[sockets[index]][stan_i]][4])/128);
                                                                    buffer2[21]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[22]= 128+(quest_moby[player_quest[sockets[index]][stan_i]][4])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_moby[player_quest[sockets[index]][stan_i]][5])/128);
                                                                    buffer2[23]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[24]= 128+(quest_moby[player_quest[sockets[index]][stan_i]][5])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_moby[player_quest[sockets[index]][stan_i]][6])/128);
                                                                    buffer2[25]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[26]= 128+(quest_moby[player_quest[sockets[index]][stan_i]][6])-(tymczasowa*128);
                                                                    tymczasowa = ((quest_moby[player_quest[sockets[index]][stan_i]][7])/128);
                                                                    buffer2[27]= 128+tymczasowa;   		//numer id questa
                                                                    buffer2[28]= 128+(quest_moby[player_quest[sockets[index]][stan_i]][7])-(tymczasowa*128);

                                                                    buffer2[29]= 128+player_quest_finnish[sockets[index]][stan_i][0];
                                                                    buffer2[30]= 128+player_quest_finnish[sockets[index]][stan_i][1];
                                                                    buffer2[31]= 128+player_quest_finnish[sockets[index]][stan_i][2];
                                                                    buffer2[32]= 128+player_quest_finnish[sockets[index]][stan_i][3];
                                                                    buffer2[33]= 128+player_quest_finnish[sockets[index]][stan_i][4];
                                                                    buffer2[34]= 128+player_quest_finnish[sockets[index]][stan_i][5];
                                                                    buffer2[35]= 128+player_quest_finnish[sockets[index]][stan_i][6];
                                                                    buffer2[36]= 128+player_quest_finnish[sockets[index]][stan_i][7];
                                                                    buffer2[37]= 128+quest_moby_ilosc[player_quest[sockets[index]][stan_i]][0];
                                                                    buffer2[38]= 128+quest_moby_ilosc[player_quest[sockets[index]][stan_i]][0];
                                                                    buffer2[39]= 128+quest_moby_ilosc[player_quest[sockets[index]][stan_i]][0];
                                                                    buffer2[40]= 128+quest_moby_ilosc[player_quest[sockets[index]][stan_i]][0];
                                                                    buffer2[41]= 128+quest_moby_ilosc[player_quest[sockets[index]][stan_i]][0];
                                                                    buffer2[42]= 128+quest_moby_ilosc[player_quest[sockets[index]][stan_i]][0];
                                                                    buffer2[43]= 128+quest_moby_ilosc[player_quest[sockets[index]][stan_i]][0];
                                                                    buffer2[44]= 128+quest_moby_ilosc[player_quest[sockets[index]][stan_i]][0];
                                                                    buffer2[45]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][0];
                                                                    buffer2[46]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][1];
                                                                    buffer2[47]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][2];
                                                                    buffer2[48]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][3];
                                                                    buffer2[49]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][4];
                                                                    buffer2[50]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][5];
                                                                    buffer2[51]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][6];
                                                                    buffer2[52]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][7];
                                                                    buffer2[53]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][8];
                                                                    buffer2[54]= 128+quest_oddaj[player_quest[sockets[index]][stan_i]][9];
                                                                    buffer2[55]= 13;
                                                                    buffer2[56]= 10;
                                                                    result_s = send(sockets[j], buffer2, 56, 0);
                                                                    bytes_sent[(sockets[j])] += 56;


                                                                }
                                                            }
                                                        }
                                                    }
                                                    else {  							//grupa mobkow do 1. licznika
                                                        for(stan_m=0; stan_m<8; stan_m++) {
                                                            if(quest_moby[player_quest[sockets[index]][stan_i]][stan_m]==mob_index[player_map[sockets[index]]][spelle_mob[sockets[index]][0]-(64*128)]) {

                                                                if( (player_quest_finnish[sockets[index]][stan_i][0]<quest_dozabicia[player_quest[sockets[index]][stan_i]]) && (temp_dozabicia==0) ) {
                                                                    temp_dozabicia=1;
                                                                    player_quest_finnish[sockets[index]][stan_i][0]++;
                                                                    printf("quest: %d   rozne moby, znaleziony mob nr: %d   ilosc %d/%d\n",player_quest[sockets[index]][stan_i],quest_moby[player_quest[sockets[index]][stan_i]][stan_m],player_quest_finnish[sockets[index]][stan_i][0],quest_dozabicia[player_quest[sockets[index]][stan_i]]);
                                                                    //do_zabicia=quest_dozabicia[player_quest[sockets[index]][stan_i]];
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    //wysylanie informacji ze dany mob otrzymal DMG
                                    buffer2[0]= 68;  //D - DMG
                                    tymczasowa=((spelle_mob[sockets[index]][0])/128);
                                    buffer2[1]= 128+tymczasowa;
                                    buffer2[2]= 128+spelle_mob[sockets[index]][0]-(tymczasowa*128);
                                    tymczasowa=(hit/128);
                                    buffer2[3]= 128+tymczasowa;
                                    buffer2[4]= 128+hit-(tymczasowa*128);
                                    tymczasowa=(zysk_expa/128);
                                    buffer2[5]= 128+tymczasowa;
                                    buffer2[6]= 128+zysk_expa-(tymczasowa*128);
                                    buffer2[7]= 13;
                                    buffer2[8]= 10;
                                    result_s = -666;
                                    for (j=0; j<sockets_index; j++) {
                                        result_s = send(sockets[j], buffer2, 9, 0);
                                        bytes_sent[(sockets[j])] += 7;
                                    }
                                    player_info[sockets[index]]=1;
                                    player_stan[sockets[index]][index]=0;
                                    buffer2[1]=128+3; // 0 - rozpoczecie 1 - cofniecie 2- przerwanie 3-zakonczenie
                                } else {
                                    buffer2[1]=128+2; // 0 - rozpoczecie 1 - cofniecie 2- przerwanie 3-zakonczenie
                                }
                                buffer2[0]=65; //A (65) - atak, informacja o cascie
                                buffer2[2]= 13;
                                buffer2[3]= 10;
                                result_s = send(sockets[index], buffer2, 4, 0);
                                //anulowanie/zakonczenie danego ataku
                                for(j=0; j<(spelle_l[sockets[index]]-1); j++) {
                                    spelle[sockets[index]][j]=spelle[sockets[index]][j+1];
                                    spelle_mob[sockets[index]][j]=spelle_mob[sockets[index]][j+1];
                                }
                                spelle_l[sockets[index]]--;
                                if(spelle_l[sockets[index]]>0) {
                                    spelle_endwait[sockets[index]] = getTime() + casttime(spelle[sockets[index]][0]) ; //czas casta
                                }
                            } //koniec instrukcji jesli target to mob
                            // tu mozna dodac co jesli targetem jest gracz
                            break;
                        case 102:
                            if(rodzaj_postaci==1) {  //jesli to gracz

                            }
                            else { //jesli to ktos inny to heala samego siebie
                                spelle_mob[sockets[index]][0]=sockets[index]+4096;
                            }
                            //HEAL
                            printf("AAAA: %d %d\n", sockets[index], spelle_mob[sockets[index]][0]);
                            player_czas_cast[sockets[index]]=(4*6)/cycleX; 		//6 sekund bez casta to pelny regen
                            player_mp[sockets[index]]-=koszt_spella_kopia;
                            for(stan_i=0; stan_i<sockets_index; stan_i++) //sprawdzic czy jest ok!!!
                                player_stan[sockets[index]][stan_i]=0;
                            int hit=wylicz_hit(spelle[sockets[index]][0], ((player_lvl[sockets[index]]+1)/2));
                            player_hp[spelle_mob[sockets[index]][0]-(32*128)]+=hit;
                            for(stan_i=0; stan_i<sockets_index; stan_i++) //wyslac informace wszystkim o leczonym graczu
                                player_stan[spelle_mob[sockets[index]][0]-(32*128)][stan_i]=0;
                            if(player_hp[spelle_mob[sockets[index]][0]-(32*128)]>player_hp_max[spelle_mob[sockets[index]][0]-(32*128)]) {
                                player_hp[spelle_mob[sockets[index]][0]-(32*128)]=player_hp_max[spelle_mob[sockets[index]][0]-(32*128)];
                            }
                            //wysylanie informacji ze dany gracz otrzymal -DMG
                            buffer2[0]= 68;  //D - DMG
                            tymczasowa=((spelle_mob[sockets[index]][0])/128);
                            buffer2[1]= 128+tymczasowa;
                            buffer2[2]= 128+spelle_mob[sockets[index]][0]-(tymczasowa*128);
                            tymczasowa=(hit/128);
                            buffer2[3]= 128+tymczasowa;
                            buffer2[4]= 128+hit-(tymczasowa*128);
                            tymczasowa=(zysk_expa/128);
                            buffer2[5]= 127;
                            buffer2[6]= 127; //oznacza to ze nie chodzi o dmg a o heal
                            buffer2[7]= 13;
                            buffer2[8]= 10;
                            result_s = -666;
                            for (j=0; j<sockets_index; j++) {
                                result_s = send(sockets[j], buffer2, 9, 0);
                                bytes_sent[(sockets[j])] += 7;
                            }
                            player_info[sockets[index]]=1;
                            player_stan[sockets[index]][index]=0;

                            buffer2[0]=65; //A (65) - atak, informacja o cascie
                            buffer2[1]=128+3; // 0 - rozpoczecie 1 - cofniecie 2- przerwanie 3-zakonczenie
                            buffer2[2]= 13;
                            buffer2[3]= 10;
                            result_s = send(sockets[index], buffer2, 4, 0);
                            for(j=0; j<(spelle_l[sockets[index]]-1); j++) {
                                spelle[sockets[index]][j]=spelle[sockets[index]][j+1];
                                spelle_mob[sockets[index]][j]=spelle_mob[sockets[index]][j+1];
                            }
                            spelle_l[sockets[index]]--;
                            if(spelle_l[sockets[index]]>0) {
                                spelle_endwait[sockets[index]] = getTime() + casttime(spelle[sockets[index]][0]) ; //czas casta
                            }
                            break;
                        } //koniec switch
                    }
                }
                else {
                    buffer2[0]=65; //A (65) - atak, informacja o cascie
                    buffer2[1]=128+2; // 0 - rozpoczecie 1 - cofniecie 2- przerwanie 3-zakonczenie
                    buffer2[2]= 13;
                    buffer2[3]= 10;
                    result_s = send(sockets[index], buffer2, 4, 0);
                    //out of mana message set to player
                    //rozpoczynanie nastepnego spella
                    for(j=0; j<(spelle_l[sockets[index]]-1); j++) {
                        spelle[sockets[index]][j]=spelle[sockets[index]][j+1];
                        spelle_mob[sockets[index]][j]=spelle_mob[sockets[index]][j+1];
                    }
                    spelle_l[sockets[index]]--;
                    if(spelle_l[sockets[index]]>0) {
                        spelle_endwait[sockets[index]] = getTime() + casttime(spelle[sockets[index]][0]) ; //czas casta
                    }
                }
            }
            if(player_stan[sockets[index]][index]<2) {
                buffer2[0]=86; //V (86) - odswierz info o graczu
                tymczasowa = (player_hp[(sockets[index])]/128);
                buffer2[1]= 128+tymczasowa;   		//HP postaci
                buffer2[2]= 128+(player_hp[(sockets[index])])-tymczasowa*128;
                tymczasowa = (player_mp[(sockets[index])]/128);
                buffer2[3]= 128+tymczasowa;   		//MP postaci
                buffer2[4]= 128+(player_mp[(sockets[index])])-tymczasowa*128;
                tymczasowa = (player_hp_max[(sockets[index])]/128);
                buffer2[5]= 128+tymczasowa;   		//HP MAX postaci
                buffer2[6]= 128+(player_hp_max[(sockets[index])])-tymczasowa*128;
                tymczasowa = (player_mp_max[(sockets[index])]/128);
                buffer2[7]= 128+tymczasowa;   		//MP MAX postaci
                buffer2[8]= 128+(player_mp_max[(sockets[index])])-tymczasowa*128;
                tymczasowa = (player_exp[(sockets[index])]/128);
                buffer2[9]= 128+tymczasowa;   		//EXP postaci
                buffer2[10]= 128+(player_exp[(sockets[index])])-tymczasowa*128;
                tymczasowa = (player_lvl[(sockets[index])]/128);
                buffer2[11]= 128+tymczasowa;   		//MP postaci
                buffer2[12]= 128+(player_lvl[(sockets[index])])-tymczasowa*128;
                buffer2[13]= 13;
                buffer2[14]= 10;
                result_s = send(sockets[index], buffer2, 15, 0);
                player_info[sockets[index]]=0;
                player_stan[sockets[index]][index]=2; //wyslany stan
            }


        }
        for (index = 0; index < sockets_waiting_index; index++) {
            if (odpowiedz[sockets_waiting[index]]<-20) {
                //WYLOGOWANIE KLIENTA - tu nie trzeba nic wysylac bo klient w zasadzie nei jest zalogowany
                sockets_waiting_index--;
                for(i=index; i<sockets_waiting_index; i++)
                    sockets_waiting[i]=sockets_waiting[i+1];
                //KONIEC WYLOGOWYWANIA
            }
        }

        switch (result) {

        /* error in select */
        case -1:
            perror("select");
            break;

        /* nothing to process */
        case 0:
            break;

        /* a number of sockets are ready for reading */
        default:
            /* check if the descriptor set is our listening one */

            if (FD_ISSET(descriptor , &input)) {
                sockets_waiting[sockets_waiting_index] = accept(descriptor, NULL, NULL);
                //sockets[sockets_index] = accept(descriptor, NULL, NULL);
                if ((sockets_waiting[sockets_waiting_index] == -1) || ((sockets_index+sockets_waiting_index)==D_SOCKETS) ) {
                    perror("accept");
                }
                else {  //connecting client
                    if (sockets_waiting[sockets_waiting_index] > maximun)
                        maximun = sockets_waiting[sockets_waiting_index];
                    //client[(sockets[sockets_index])]=sockets_index;
                    //player_map[sockets[sockets_index]]=1;
                    //printf("dodaje klienta: %d  :  %d\n", sockets_waiting[sockets_waiting_index], sockets_waiting_index);
                    odpowiedz[(sockets_waiting[sockets_waiting_index])]=0;
                    sockets_waiting_index++;
                }
            }
            /* one of the sockets is sending data. Find it */
            else {
                for (index = 0; index < sockets_index; index++) {
                    if (FD_ISSET(sockets[index], &input)) {
                        memset(buffer, 0, sizeof(buffer));

                        //result = recv(sockets[index], buffer, sizeof(buffer), 0);
                        result = recv(sockets[index], buffer, 10, 0);
                        if ( (result == -1) || (result == 0) ) {
                            //WYLOGOWANIE KLIENTA

                            //zapisywanie pozycji
                            konto_start[nr_konta[sockets[index]]][0]=player_map[(sockets[index])];
                            konto_start[nr_konta[sockets[index]]][1]=player_x[(sockets[index])];
                            konto_start[nr_konta[sockets[index]]][2]=player_y[(sockets[index])];
                            konto_hp[nr_konta[sockets[index]]]=player_hp[(sockets[index])];
                            konto_mp[nr_konta[sockets[index]]]=player_mp[(sockets[index])];
                            konto_hp_max[nr_konta[sockets[index]]]=player_hp_max[(sockets[index])];
                            konto_mp_max[nr_konta[sockets[index]]]=player_mp_max[(sockets[index])];
                            konto_exp[nr_konta[sockets[index]]]=player_exp[(sockets[index])];
                            konto_lvl[nr_konta[sockets[index]]]=player_lvl[(sockets[index])];
                            konto_klasa[nr_konta[sockets[index]]]=player_klasa[(sockets[index])];
                            konto_kasa[nr_konta[sockets[index]]]=player_kasa[(sockets[index])];
                            for(stan_i=0; stan_i<8; stan_i++)	konto_quest[nr_konta[sockets[index]]][stan_i]=player_quest[nr_konta[sockets[index]]][stan_i];
                            for(stan_m=0; stan_m<10; stan_m++) {
                                for(stan_i=0; stan_i<8; stan_i++) {
                                    konto_quest_finnish[nr_konta[sockets[index]]][stan_m][stan_i]=player_quest_finnish[nr_konta[sockets[index]]][stan_m][stan_i];
                                }
                            }
                            for(stan_i=0; stan_i<30; stan_i++)
                                konto_equip[nr_konta[sockets[index]]][stan_i]=player_equip[(sockets[index])][stan_i];
                            for(stan_i=0; stan_i<5; stan_i++)
                                konto_bagi[nr_konta[sockets[index]]][stan_i]=player_bagi[(sockets[index])][stan_i];
                            for(stan_i=0; stan_i<75; stan_i++) {
                                konto_inventory[nr_konta[sockets[index]]][stan_i]=player_inventory[(sockets[index])][stan_i];
                                konto_inventory_ilosc[nr_konta[sockets[index]]][stan_i]=player_inventory_ilosc[(sockets[index])][stan_i];
                            }
                            konto_buff_index[nr_konta[sockets[index]]]=player_buff_index[(sockets[index])];
                            for(stan_i=0; stan_i<16; stan_i++) {
                                konto_buff[nr_konta[sockets[index]]][stan_i]=player_buff[(sockets[index])][stan_i];
                                konto_buff_time[nr_konta[sockets[index]]][stan_i]=player_buff_time[(sockets[index])][stan_i]-cycle;
                                konto_buff_freq[nr_konta[sockets[index]]][stan_i]=player_buff_freq[(sockets[index])][stan_i];
                                konto_buff_interrupt[nr_konta[sockets[index]]][stan_i]=player_buff_interrupt[(sockets[index])][stan_i];
                                konto_buff_attr1[nr_konta[sockets[index]]][stan_i]=player_buff_attr1[(sockets[index])][stan_i];
                                konto_buff_attr2[nr_konta[sockets[index]]][stan_i]=player_buff_attr2[(sockets[index])][stan_i];
                            }
                            strcpy(sciezka, "login/");
                            strcat(sciezka, login[nr_konta[sockets[index]]]);
                            strcat(sciezka, "/position");
                            printf("writing to file: %s\n",sciezka);
                            plik02 = fopen(sciezka, "w" );
                            fprintf(plik02,"%d,%d,%d\n", player_map[(sockets[index])], player_x[(sockets[index])], player_y[(sockets[index])]);
                            fprintf(plik02,"%d,%d,%d,%d\n",player_lvl[(sockets[index])], player_exp[(sockets[index])], player_hp[(sockets[index])], player_mp[(sockets[index])]);
                            fprintf(plik02,"%d\n",player_klasa[(sockets[index])]);
                            fprintf(plik02,"%d\n",player_buff_index[(sockets[index])]);
                            for(i=0; i<player_buff_index[sockets[index]]; i++)
                                fprintf(plik02,"%d,%d,%d,%d,%d,%d\n",player_buff[sockets[index]][i],(player_buff_time[sockets[index]][i]-cycle),player_buff_freq[sockets[index]][i],player_buff_interrupt[sockets[index]][i],player_buff_attr1[sockets[index]][i],player_buff_attr2[sockets[index]][i]);
                            fclose(plik02);
                            strcpy(sciezka, "login/");
                            strcat(sciezka, login[nr_konta[sockets[index]]]);
                            strcat(sciezka, "/inv");
                            printf("writing to file: %s\n",sciezka);
                            plik02 = fopen(sciezka, "w" );
                            for(stan_i=0; stan_i<29; stan_i++)
                                fprintf(plik02,"%d,",player_equip[(sockets[index])][stan_i]);
                            stan_i=29;
                            fprintf(plik02,"%d\n",player_equip[(sockets[index])][stan_i]);
                            fprintf(plik02,"%d,%d,%d,%d\n", player_bagi[(sockets[index])][0],player_bagi[(sockets[index])][1],player_bagi[(sockets[index])][2],player_bagi[(sockets[index])][3]);
                            for(stan_m=0; stan_m<5; stan_m++) {
                                for(stan_i=0; stan_i<14; stan_i++)
                                    fprintf(plik02,"%d,",player_inventory[(sockets[index])][stan_i+(stan_m*15)]);
                                stan_i=14;
                                fprintf(plik02,"%d\n",player_inventory[(sockets[index])][stan_i+(stan_m*15)]);
                                for(stan_i=0; stan_i<14; stan_i++)
                                    fprintf(plik02,"%d,",player_inventory_ilosc[(sockets[index])][stan_i+(stan_m*15)]);
                                stan_i=14;
                                fprintf(plik02,"%d\n",player_inventory_ilosc[(sockets[index])][stan_i+(stan_m*15)]);
                            }
                            fprintf(plik02,"%d\n",player_kasa[(sockets[index])]);
                            fclose(plik02);
                            strcpy(sciezka, "login/");
                            strcat(sciezka, login[nr_konta[sockets[index]]]);
                            strcat(sciezka, "/quest");
                            printf("writing to file: %s\n",sciezka);
                            plik02 = fopen(sciezka, "w" );
                            for(stan_i=0; stan_i<9; stan_i++)	fprintf(plik02,"%d,",player_quest[(sockets[index])][stan_i]);
                            fprintf(plik02,"%d\n",player_quest[(sockets[index])][9]);
                            for(stan_m=0; stan_m<10; stan_m++) {
                                for(stan_i=0; stan_i<8; stan_i++) {
                                    if(stan_i!=7)
                                        fprintf(plik02,"%d,",player_quest_finnish[(sockets[index])][stan_m][stan_i]);
                                    else
                                        fprintf(plik02,"%d\n",player_quest_finnish[(sockets[index])][stan_m][stan_i]);
                                }
                            }
                            fclose(plik02);
                            //koniec zapisywania pozycji

                            odpowiedz[(sockets[index])]=0;
                            buffer2[0]= 80;  //P - postac
                            tymczasowa=(sockets[index]/128);
                            buffer2[1]= 128+32+tymczasowa;
                            buffer2[2]= 128+sockets[index]-(tymczasowa*128);
                            buffer2[3]= 68;  //R - ruch Q - nick D68 - usuniecie
                            player_x[(sockets[index])]=-1;
                            player_y[(sockets[index])]=-1;
                            player_map[(sockets[index])]=-1;
                            buffer2[4]= 128;
                            buffer2[5]= 127;
                            buffer2[6]= 128;
                            buffer2[7]= 127;
                            buffer2[8]= 128+1;
                            buffer2[9]= 128;
                            for(n=0; n<10; n++)
                                buffer2[10+n]= 32;
                            buffer2[20]= 13;
                            buffer2[21]= 10;
                            result_s = -666;
                            for (j=0; j<sockets_index; j++) {
                                if(sockets[j]!=sockets[index]) {
                                    result_s = send(sockets[j], buffer2, 22, 0);
                                    bytes_sent[(sockets[j])] += 22;
                                }
                            }
                            for(stan_m=1; stan_m<(il_map+1); stan_m++) {
                                for(stan_i=0; stan_i<mob_max[stan_m]; stan_i++) {
                                    if(mob_target[stan_m][stan_i]==sockets[index]) {
                                        mob_target[stan_m][stan_i]=-1;
                                        printf("OTRZYMANO SYGNAL! %d\n",sockets[index]);
                                    }
                                }
                            }
                            sockets_index--;
                            bytes_sent[(sockets[index])]=0;
                            for(i=index; i<sockets_index; i++) {
                                sockets[i]=sockets[i+1];
                                for(stan_m=1; stan_m<(il_map+1); stan_m++) { //wszystko co sie rusza przesuwamy
                                    for(stan_i=0; stan_i<npc_max[stan_m]; stan_i++)
                                        npc_stan[stan_m][stan_i][i]=npc_stan[stan_m][stan_i][i+1];
                                    for(stan_i=0; stan_i<mob_max[stan_m]; stan_i++)
                                        mob_stan[stan_m][stan_i][i]=mob_stan[stan_m][stan_i][i+1];
                                }
                                for(stan_i=0; stan_i<sockets_index; stan_i++)
                                    player_stan[sockets[stan_i]][i]=player_stan[sockets[stan_i]][i+1];
                            }
                            //KONIEC WYLOGOWYWANIA
                        }
                        else {  // TRANSMISJE <<<<<<<<<<<<<

                            //printf("Received %d bytes from descriptor %d: %s\n", result, sockets[index], buffer);
                            //printf("Received %d bytes from descriptor %d: %d %d / %d %d %d \n", result, sockets[index], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
                            int zle_id = 1;
                            if((result==10) && (buffer[0]==67)) {   // C - pozycja
                                odpowiedz[(sockets[index])]=1;
                                if( (player_x[sockets[index]]!=rip2_x(buffer)) || (player_y[sockets[index]]!=rip2_y(buffer)) ) {
                                    for(stan_i=0; stan_i<sockets_index; stan_i++)
                                        player_stan[sockets[index]][stan_i]=0;
                                }
                                if((player_x[sockets[index]]!=rip2_x(buffer)) || (player_y[sockets[index]]!=rip2_y(buffer)))
                                    player_ruch[sockets[index]]=1;
                                player_x[sockets[index]]=rip2_x(buffer);
                                player_y[sockets[index]]=rip2_y(buffer);
                                //player_map[sockets[index]]=1;
                                //printf("p%d : %d %d  -  %d\n",index,(player_x[(sockets[index])]),(player_y[(sockets[index])]),sockets[index]);
                                zle_id=0;
                            }
                            if((result==10) && (buffer[0]==65)) {   // A - atak (cast spella/skilla)
                                //printf("fire - p%d : %d %d\n",index,((buffer[1]-64)*64)+(buffer[2]-64),((buffer[3]-64)*64*64)+((buffer[4]-64)*64)+(buffer[5]-64));
                                player_ruch[sockets[index]]=1;
                                if(buffer[1]!=63) {
                                    if(spelle_l[sockets[index]]<3) {
                                        spelle[sockets[index]][spelle_l[sockets[index]]]=((buffer[1]-64)*64)+(buffer[2]-64);
                                        spelle_mob[sockets[index]][spelle_l[sockets[index]]]=((buffer[3]-64)*64*64)+((buffer[4]-64)*64)+(buffer[5]-64);
                                        if(spelle_l[sockets[index]]==0) {
                                            spelle_endwait[sockets[index]] = getTime () + casttime(spelle[sockets[index]][0]) ; //czas casta
                                            buffer2[0]=65; //A (65) - atak, informacja o rozpoczeciu casta
                                            buffer2[1]=128+0; // 0 - rozpoczecie 1 - cofniecie 2- przerwanie 3-zakonczenie
                                            buffer2[2]= 13;
                                            buffer2[3]= 10;
                                            result_s = send(sockets[index], buffer2, 4, 0);
                                        }
                                        spelle_l[sockets[index]]++;
                                    }
                                } else {
                                    spelle_l[sockets[index]]=0;
                                }
                                zle_id=0;
                            }
                            if((result==10) && (buffer[0]==73)) {   // I - inventory
                                buffer2[0]=73; //I (73) - odpowiedz na inventory
                                for(i=0; i<4; i++) {
                                    buffer2[1+i]=128+player_bagi[sockets[index]][i]; //1-4
                                }
                                tymczasowa = ((player_kasa[sockets[index]]/128)/128);
                                tymczasowa2 = player_kasa[sockets[index]]/128;
                                buffer2[5]= 128+tymczasowa;   		//socket logujacego sie gracza na serwerze
                                buffer2[6]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[7]= 128+(player_kasa[sockets[index]])-(tymczasowa2*128)-(tymczasowa*128*128);
                                int start_trans=8;
                                for(i=0; i<75; i++) {
                                    tymczasowa = ((player_inventory[sockets[index]][i]/128)/128);
                                    tymczasowa2 = player_inventory[sockets[index]][i]/128;
                                    buffer2[start_trans+(i*3)+0]= 128+tymczasowa;
                                    buffer2[start_trans+(i*3)+1]= 128+tymczasowa2-(tymczasowa*128);
                                    buffer2[start_trans+(i*3)+2]= 128+(player_inventory[sockets[index]][i])-(tymczasowa2*128)-(tymczasowa*128*128);
                                }
                                buffer2[233]= 13;
                                buffer2[234]= 10;
                                result_s = send(sockets[index], buffer2, 235, 0);

                                buffer2[0]=74; //J (74) - informacje o grafice/ilosci
                                buffer2[1]=128;  //grafika
                                for(i=0; i<75; i++) {
                                    tymczasowa = item_grafika[player_inventory[sockets[index]][i]]/128;
                                    buffer2[2+(i*2)+0]= 128+tymczasowa;
                                    buffer2[2+(i*2)+1]= 128+item_grafika[player_inventory[sockets[index]][i]]-(tymczasowa*128);
                                }
                                buffer2[152]= 13;
                                buffer2[153]= 10;
                                result_s = send(sockets[index], buffer2, 154, 0);

                                buffer2[0]=74; //J (74) - informacje o grafice/ilosci
                                buffer2[1]=128+1; //ilosc
                                for(i=0; i<75; i++) {
                                    tymczasowa = player_inventory_ilosc[sockets[index]][i]/128;
                                    buffer2[2+(i*2)+0]= 128+tymczasowa;
                                    buffer2[2+(i*2)+1]= 128+player_inventory_ilosc[sockets[index]][i]-(tymczasowa*128);
                                }
                                buffer2[152]= 13;
                                buffer2[153]= 10;
                                result_s = send(sockets[index], buffer2, 154, 0);
                                zle_id=0;
                            }
                            if((result==10) && (buffer[0]==69)) {   // E - equip
                                buffer2[0]=69; //E (69) - odpowiedz na equip
                                for(i=0; i<30; i++) {
                                    tymczasowa = ((player_equip[sockets[index]][i]/128)/128);
                                    tymczasowa2 = player_equip[sockets[index]][i]/128;
                                    buffer2[1+(i*3)+0]= 128+tymczasowa;
                                    buffer2[1+(i*3)+1]= 128+tymczasowa2-(tymczasowa*128);
                                    buffer2[1+(i*3)+2]= 128+(player_equip[sockets[index]][i])-(tymczasowa2*128)-(tymczasowa*128*128);
                                }
                                buffer2[91]= 13;
                                buffer2[92]= 10;
                                result_s = send(sockets[index], buffer2, 93, 0);

                                buffer2[0]=70; //F (70) - informacje o grafice
                                buffer2[1]=128;  //grafika
                                for(i=0; i<30; i++) {
                                    tymczasowa = item_grafika[player_equip[sockets[index]][i]]/128;
                                    buffer2[2+(i*2)+0]= 128+tymczasowa;
                                    buffer2[2+(i*2)+1]= 128+item_grafika[player_equip[sockets[index]][i]]-(tymczasowa*128);
                                }
                                buffer2[62]= 13;
                                buffer2[63]= 10;
                                result_s = send(sockets[index], buffer2, 64, 0);
                                zle_id=0;
                            }
                            if((result==10) && (buffer[0]==79)) {   // O - info o itemce
                                odpowiedz[(sockets[index])]=1;
                                int nr_itemu = ((buffer[1]-64)*(64*64))+((buffer[2]-64)*(64))+(buffer[3]-64);
                                buffer2[0]=79; //O (79) - informacje o pojedynczej itemce
                                tymczasowa = (item_typ[nr_itemu]/128)/128;
                                tymczasowa2 = item_typ[nr_itemu]/128;
                                buffer2[1]= 128+tymczasowa;
                                buffer2[2]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[3]= 128+(item_typ[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_wartosc[nr_itemu]/128)/128;
                                tymczasowa2 = item_wartosc[nr_itemu]/128;
                                buffer2[4]= 128+tymczasowa;
                                buffer2[5]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[6]= 128+(item_wartosc[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_dura[nr_itemu]/128)/128;
                                tymczasowa2 = item_dura[nr_itemu]/128;
                                buffer2[7]= 128+tymczasowa;
                                buffer2[8]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[9]= 128+(item_dura[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_stack[nr_itemu]/128)/128;
                                tymczasowa2 = item_stack[nr_itemu]/128;
                                buffer2[10]= 128+tymczasowa;
                                buffer2[11]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[12]= 128+(item_stack[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_arm[nr_itemu]/128)/128;
                                tymczasowa2 = item_arm[nr_itemu]/128;
                                buffer2[13]= 128+tymczasowa;
                                buffer2[14]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[15]= 128+(item_arm[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_md[nr_itemu]/128)/128;
                                tymczasowa2 = item_md[nr_itemu]/128;
                                buffer2[16]= 128+tymczasowa;
                                buffer2[17]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[18]= 128+(item_md[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_bd[nr_itemu]/128)/128;
                                tymczasowa2 = item_bd[nr_itemu]/128;
                                buffer2[19]= 128+tymczasowa;
                                buffer2[20]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[21]= 128+(item_bd[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_bh[nr_itemu]/128)/128;
                                tymczasowa2 = item_bh[nr_itemu]/128;
                                buffer2[22]= 128+tymczasowa;
                                buffer2[23]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[24]= 128+(item_bh[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_int[nr_itemu]/128)/128;
                                tymczasowa2 = item_int[nr_itemu]/128;
                                buffer2[25]= 128+tymczasowa;
                                buffer2[26]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[27]= 128+(item_int[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_spi[nr_itemu]/128)/128;
                                tymczasowa2 = item_spi[nr_itemu]/128;
                                buffer2[28]= 128+tymczasowa;
                                buffer2[29]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[30]= 128+(item_spi[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_sta[nr_itemu]/128)/128;
                                tymczasowa2 = item_sta[nr_itemu]/128;
                                buffer2[31]= 128+tymczasowa;
                                buffer2[32]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[33]= 128+(item_sta[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_agi[nr_itemu]/128)/128;
                                tymczasowa2 = item_agi[nr_itemu]/128;
                                buffer2[34]= 128+tymczasowa;
                                buffer2[35]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[36]= 128+(item_agi[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (item_str[nr_itemu]/128)/128;
                                tymczasowa2 = item_str[nr_itemu]/128;
                                buffer2[37]= 128+tymczasowa;
                                buffer2[38]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[39]= 128+(item_str[nr_itemu])-(tymczasowa2*128)-(tymczasowa*128*128);
                                tymczasowa = (nr_itemu/128)/128;
                                tymczasowa2 = nr_itemu/128;
                                buffer2[40]= 128+tymczasowa;
                                buffer2[41]= 128+tymczasowa2-(tymczasowa*128);
                                buffer2[42]= 128+nr_itemu-(tymczasowa2*128)-(tymczasowa*128*128);
                                for(i=0; i<30; i++)
                                    buffer2[43+i]=item_nazwa[nr_itemu][i];
                                buffer2[73]= 13;
                                buffer2[74]= 10;
                                result_s = send(sockets[index], buffer2, 75, 0);
                                printf("ITEM:%d\n",nr_itemu);
                                zle_id=0;
                            }
                            if((result==10) && (buffer[0]==86)) {   // V - usun itemke
                                int pozycja_menu = (buffer[1]-64)+((buffer[2]-64)*5);
                                player_inventory[sockets[index]][pozycja_menu]=0;
                                player_inventory_ilosc[sockets[index]][pozycja_menu]=0;
                                porzadkuj_inventory(sockets[index]);
                            }
                            if((result==10) && (buffer[0]==85)) {   // U - uzyj itemki
                                //dodac - nie wiecej jak 16 buffow (index 15 max)
                                int pozycja_menu = (buffer[1]-64)+((buffer[2]-64)*5);
                                int item_bufor = 0;
                                switch(item_typ[player_inventory[sockets[index]][pozycja_menu]]) {
                                case 1:
                                    znaleziono_buffa=0;
                                    for(i=0; i<player_buff_index[sockets[index]]; i++) {
                                        if(player_buff[sockets[index]][i]=item_typ[player_inventory[sockets[index]][pozycja_menu]])
                                            znaleziono_buffa=1;
                                    }
                                    if(znaleziono_buffa==0) {
                                        player_buff[sockets[index]][player_buff_index[sockets[index]]] = 1; //regen - musi sie zgadzac z typem itemki!!
                                        player_buff_attr1[sockets[index]][player_buff_index[sockets[index]]] = item_int[player_inventory[sockets[index]][pozycja_menu]]; //intelekt
                                        player_buff_attr2[sockets[index]][player_buff_index[sockets[index]]] = item_sta[player_inventory[sockets[index]][pozycja_menu]]; //stamina
                                        player_buff_time[sockets[index]][player_buff_index[sockets[index]]] = cycle+40; //czas w cyklach (20sek)
                                        player_buff_freq[sockets[index]][player_buff_index[sockets[index]]] = 8; //czas w cyklach
                                        player_buff_interrupt[sockets[index]][player_buff_index[sockets[index]]] = 1; //1-tak 0-nie
                                        player_buff_index[sockets[index]]++;
                                        player_ruch[sockets[index]]=0;
                                        buffer2[0]=66; //B (66) - informacje o buffie
                                        tymczasowa = player_buff[sockets[index]][player_buff_index[sockets[index]]-1]/128;
                                        buffer2[1]= 128+tymczasowa;
                                        buffer2[2]= 128+(player_buff[sockets[index]][player_buff_index[sockets[index]]-1])-(tymczasowa*128);
                                        tymczasowa = (player_buff_time[sockets[index]][player_buff_index[sockets[index]]-1]-cycle)/128;
                                        buffer2[3]= 128+tymczasowa;
                                        buffer2[4]= 128+(player_buff_time[sockets[index]][player_buff_index[sockets[index]]-1]-cycle)-(tymczasowa*128);
                                        buffer2[5]= 129;
                                        buffer2[6]= 13;
                                        buffer2[7]= 10;
                                        result_s = send(sockets[index], buffer2, 8, 0);
                                        player_inventory_ilosc[sockets[index]][pozycja_menu]--;
                                        if(player_inventory_ilosc[sockets[index]][pozycja_menu]<1) {
                                            player_inventory_ilosc[sockets[index]][pozycja_menu]=0;
                                            player_inventory[sockets[index]][pozycja_menu]=0;
                                        }
                                    }
                                    break; //porzadkuj_inventory(sockets[index]);
                                case 20:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][0];
                                        player_equip[sockets[index]][0] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 21:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][1];
                                        player_equip[sockets[index]][1] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 22:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][2];
                                        player_equip[sockets[index]][2] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 23:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][3];
                                        player_equip[sockets[index]][3] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 24:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][4];
                                        player_equip[sockets[index]][4] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 25:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][5];
                                        player_equip[sockets[index]][5] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 26:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][6];
                                        player_equip[sockets[index]][6] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 27:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][7];
                                        player_equip[sockets[index]][7] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 28:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][8];
                                        player_equip[sockets[index]][8] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 29:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][9];
                                        player_equip[sockets[index]][9] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 30:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][10];
                                        player_equip[sockets[index]][10] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 31:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][11];
                                        player_equip[sockets[index]][11] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 32:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][12];
                                        player_equip[sockets[index]][12] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 33:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][13];
                                        player_equip[sockets[index]][13] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 34:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][14];
                                        player_equip[sockets[index]][14] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 35:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][15];
                                        player_equip[sockets[index]][15] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 36:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][16];
                                        player_equip[sockets[index]][16] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 37:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][17];
                                        player_equip[sockets[index]][17] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 38:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][18];
                                        player_equip[sockets[index]][18] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                case 39:
                                    if(item_dura[(player_inventory[sockets[index]][pozycja_menu])]<=player_lvl[sockets[index]]) {
                                        item_bufor=player_inventory[sockets[index]][pozycja_menu];
                                        player_inventory[sockets[index]][pozycja_menu]=player_equip[sockets[index]][19];
                                        player_equip[sockets[index]][19] = item_bufor;
                                        porzadkuj_inventory(sockets[index]);
                                        break;
                                    }
                                }
                                if(player_inventory[sockets[index]][pozycja_menu]==0) player_inventory_ilosc[sockets[index]][pozycja_menu]=0; //jesli jakis przedmiot zostal ubrany/zuzyty wyzeruj ilosc
                                if((item_typ[player_inventory[sockets[index]][pozycja_menu]]>=20) && (item_typ[player_inventory[sockets[index]][pozycja_menu]]<=39)) {
                                    wylicz_staty(player_klasa[sockets[index]],player_lvl[sockets[index]],sockets[index]);
                                    player_hp_max[sockets[index]]=w_stamina*14;
                                    player_mp_max[sockets[index]]=w_intelect*10;
                                    player_spirit[sockets[index]]=w_spirit;
                                }
                                zle_id=0;
                            }
                            if((result==10) && (buffer[0]==76)) {   // L - loot asdf
                                for(i=0; i<mob_max[player_map[sockets[index]]]; i++) { //mobki
                                    if(mob_hp[player_map[sockets[index]]][i]==0) {
                                        int x_mobka=mob_pos_x[player_map[sockets[index]]][i]/klocki;
                                        int y_mobka=mob_pos_y[player_map[sockets[index]]][i]/klocki;
                                        int x_gracza=player_x[sockets[index]];
                                        int y_gracza=player_y[sockets[index]];
                                        if((x_mobka==x_gracza) && (y_mobka==y_gracza) && (mob_target[player_map[sockets[index]]][i]==sockets[index])) {
                                            int gen_loot[8];
                                            int gen_loot_index=0;
                                            for(j=0; j<mob_loot_il[player_map[sockets[index]]][i]; j++) {
                                                if(random(10000)<mob_loot_szansa[player_map[sockets[index]]][i][j]) {
                                                    if(gen_loot_index<7) {
                                                        gen_loot[gen_loot_index]=mob_loot_index[player_map[sockets[index]]][i][j];
                                                        buffer2[0]=76; //L (76) - LOOT
                                                        tymczasowa = (gen_loot[gen_loot_index]/128)/128;
                                                        tymczasowa2 = gen_loot[gen_loot_index]/128;
                                                        buffer2[1]= 128+tymczasowa;
                                                        buffer2[2]= 128+tymczasowa2-(tymczasowa*128);
                                                        buffer2[3]= 128+(gen_loot[gen_loot_index])-(tymczasowa2*128)-(tymczasowa*128*128);
                                                        buffer2[4]= 13;
                                                        buffer2[5]= 10;
                                                        result_s = send(sockets[index], buffer2, 6, 0);
                                                        gen_loot_index++;
                                                    }
                                                }
                                            }
                                            for(j=0; j<gen_loot_index; j++) {
                                                int inv_znaleziono=0;
                                                for(k=0; k<75; k++) {
                                                    if( (player_inventory[sockets[index]][k]==gen_loot[j]) && (player_inventory_ilosc[sockets[index]][k]<item_stack[gen_loot[j]]) && (inv_znaleziono==0) ) {
                                                        player_inventory_ilosc[sockets[index]][k]++;
                                                        inv_znaleziono=1;
                                                    }
                                                }
                                                if(inv_znaleziono==0) {
                                                    int ilosc_slotow=15+player_bagi[sockets[index]][0]+player_bagi[sockets[index]][1]+player_bagi[sockets[index]][2]+player_bagi[sockets[index]][3];
                                                    for(k=0; k<75; k++) {
                                                        if( (player_inventory[sockets[index]][k]==0) && (inv_znaleziono==0) && (k<ilosc_slotow) ) {
                                                            player_inventory[sockets[index]][k]=gen_loot[j];
                                                            player_inventory_ilosc[sockets[index]][k]=1;
                                                            inv_znaleziono=1;
                                                        }
                                                    }
                                                }
                                            }
                                            mob_target[player_map[sockets[index]]][i]=-1;
                                            printf("Podnies loot! ilosc lootu:%d - 1. loot: %d \n",gen_loot_index,gen_loot[0]);
                                        }
                                    }
                                }
                                odpowiedz[(sockets[index])]=1;
                                zle_id=0;
                            }
                            if((result==10) && (buffer[0]==82)) {   // R - ozywianie sie
                                odpowiedz[(sockets[index])]=1;
                                if( (player_x[sockets[index]]!=rip2_x(buffer)) || (player_y[sockets[index]]!=rip2_y(buffer)) ) {
                                    for(stan_i=0; stan_i<sockets_index; stan_i++)
                                        player_stan[sockets[index]][stan_i]=0;
                                }
                                player_revive[sockets[index]]=1;
                                zle_id=0;
                            }
                            if(zle_id==1) {
                                printf("ZLA TRANSMISJA: %c dlugosc:%d - %d %d %d %d %d %d %d\n",buffer[0],result,buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
                            }

                        }

                    }
                }
                for (index = 0; index < sockets_waiting_index; index++) {  //logowanie 1 faza
                    if (FD_ISSET(sockets_waiting[index], &input)) {
                        memset(buffer, 0, sizeof(buffer));
                        result = recv(sockets_waiting[index], buffer, sizeof(buffer), 0);
                        if ( (result == -1) || (result == 0) ) {
                            //WYLOGOWANIE KLIENTA
                            //tu nie trzeba nic wysylac bo klient w zasadzie nei jest zalogowany
                            sockets_waiting_index--;
                            for(i=index; i<sockets_waiting_index; i++) {
                                sockets_waiting[i]=sockets_waiting[i+1];
                            }
                            //KONIEC WYLOGOWYWANIA
                        }
                        else {
                            printf("UNLOGGED - Received %d bytes from descriptor %d: %s\n", result, sockets_waiting[index], buffer);
                            if((result>3) && (buffer[0]==76)) {  //LOGOWANIE KLIENTA
                                j=sprawdz_haslo(buffer);

                                for(k=0; k<sockets_index; k++) {
                                    if(nr_konta[sockets[k]]==j)
                                        j=-1;
                                }
                                if(j>-1) {  //LOGOWANIE KLIETNA <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                    sockets[sockets_index] = sockets_waiting[index];
                                    client[(sockets[sockets_index])]=sockets_index;
                                    //player_map[sockets[sockets_index]]=1;
                                    sockets_waiting_index--;   //usuwanie uzytkownika z czekajacych
                                    for(i=index; i<sockets_waiting_index; i++) //przesuwanie uzytkownikow czekajacych
                                        sockets_waiting[i]=sockets_waiting[i+1];
                                    odpowiedz[(sockets[sockets_index])]=0;
                                    nr_konta[sockets[sockets_index]]=j;
                                    player_ekran_x[(sockets[sockets_index])]=buffer[1]-64;
                                    player_ekran_y[(sockets[sockets_index])]=buffer[2]-64;
                                    printf("x/y:   %d / %d\n",player_ekran_x[(sockets[sockets_index])],player_ekran_y[(sockets[sockets_index])]);
                                    player_map[(sockets[sockets_index])]=konto_start[nr_konta[sockets[sockets_index]]][0];
                                    player_x[(sockets[sockets_index])]=konto_start[nr_konta[sockets[sockets_index]]][1];
                                    player_y[(sockets[sockets_index])]=konto_start[nr_konta[sockets[sockets_index]]][2];
                                    player_hp[(sockets[sockets_index])]=konto_hp[nr_konta[sockets[sockets_index]]];
                                    player_mp[(sockets[sockets_index])]=konto_mp[nr_konta[sockets[sockets_index]]];
                                    player_hp_max[(sockets[sockets_index])]=konto_hp_max[nr_konta[sockets[sockets_index]]];
                                    player_mp_max[(sockets[sockets_index])]=konto_mp_max[nr_konta[sockets[sockets_index]]];
                                    player_exp[(sockets[sockets_index])]=konto_exp[nr_konta[sockets[sockets_index]]];
                                    player_lvl[(sockets[sockets_index])]=konto_lvl[nr_konta[sockets[sockets_index]]];
                                    player_klasa[(sockets[sockets_index])]=konto_klasa[nr_konta[sockets[sockets_index]]];
                                    player_spirit[(sockets[sockets_index])]=konto_spirit[nr_konta[sockets[sockets_index]]];
                                    player_kasa[(sockets[sockets_index])]=konto_kasa[nr_konta[sockets[sockets_index]]];
                                    for(stan_i=0; stan_i<10; stan_i++) {
                                        player_quest[(sockets[sockets_index])][stan_i]=konto_quest[nr_konta[sockets[sockets_index]]][stan_i];
                                    }
                                    for(stan_m=0; stan_m<10; stan_m++) {
                                        for(stan_i=0; stan_i<8; stan_i++) {
                                            player_quest_finnish[(sockets[sockets_index])][stan_m][stan_i]=konto_quest_finnish[nr_konta[sockets[sockets_index]]][stan_m][stan_i];
                                        }
                                    }
                                    for(stan_i=0; stan_i<30; stan_i++)
                                        player_equip[(sockets[sockets_index])][stan_i]=konto_equip[nr_konta[sockets[sockets_index]]][stan_i];
                                    for(stan_i=0; stan_i<5; stan_i++)
                                        player_bagi[(sockets[sockets_index])][stan_i]=konto_bagi[nr_konta[sockets[sockets_index]]][stan_i];
                                    for(stan_i=0; stan_i<75; stan_i++) {
                                        player_inventory[(sockets[sockets_index])][stan_i]=konto_inventory[nr_konta[sockets[sockets_index]]][stan_i];
                                        player_inventory_ilosc[(sockets[sockets_index])][stan_i]=konto_inventory_ilosc[nr_konta[sockets[sockets_index]]][stan_i];
                                    }
                                    player_buff_index[(sockets[sockets_index])]=konto_buff_index[nr_konta[sockets[sockets_index]]];
                                    for(stan_i=0; stan_i<16; stan_i++) {
                                        player_buff[(sockets[index])][stan_i]=konto_buff[nr_konta[sockets[index]]][stan_i];
                                        player_buff_time[(sockets[index])][stan_i]=konto_buff_time[nr_konta[sockets[index]]][stan_i]+cycle;
                                        player_buff_freq[(sockets[index])][stan_i]=konto_buff_freq[nr_konta[sockets[index]]][stan_i];
                                        player_buff_interrupt[(sockets[index])][stan_i]=konto_buff_interrupt[nr_konta[sockets[index]]][stan_i];
                                        player_buff_attr1[(sockets[index])][stan_i]=konto_buff_attr1[nr_konta[sockets[index]]][stan_i];
                                        player_buff_attr2[(sockets[index])][stan_i]=konto_buff_attr2[nr_konta[sockets[index]]][stan_i];
                                    }
                                    wylicz_staty(player_klasa[sockets[index]],player_lvl[sockets[index]],sockets[index]);
                                    player_hp_max[sockets[index]]=w_stamina*14;
                                    player_mp_max[sockets[index]]=w_intelect*10;
                                    player_spirit[sockets[index]]=w_spirit;
                                    for(stan_i=0; stan_i<sockets_index; stan_i++)
                                        player_stan[sockets[sockets_index]][stan_i]=0;
                                    for(stan_m=1; stan_m<(il_map+1); stan_m++) {
                                        for(stan_i=0; stan_i<npc_max[stan_m]; stan_i++)
                                            npc_stan[stan_m][stan_i][sockets_index]=0;
                                        for(stan_i=0; stan_i<mob_max[stan_m]; stan_i++)
                                            mob_stan[stan_m][stan_i][sockets_index]=0;
                                    }
                                    for(stan_i=0; stan_i<sockets_index; stan_i++)
                                        player_stan[sockets[stan_i]][sockets_index]=0;
                                    buffer2[0]=87; //W (87) - witaj
                                    buffer2[1]=version+128;  //wersja 0.00-1.27
                                    tymczasowa = (player_map[(sockets[sockets_index])]/128);
                                    buffer2[2]= 128+tymczasowa;   		//mapa
                                    buffer2[3]= 128+(player_map[(sockets[sockets_index])])-tymczasowa*128;
                                    tymczasowa = (player_x[(sockets[sockets_index])]/128);
                                    buffer2[4]= 128+tymczasowa;   		//X postaci
                                    buffer2[5]= 128+(player_x[(sockets[sockets_index])])-tymczasowa*128;
                                    tymczasowa = (player_y[(sockets[sockets_index])]/128);
                                    buffer2[6]= 128+tymczasowa;   		//Y postaci
                                    buffer2[7]= 128+(player_y[(sockets[sockets_index])])-tymczasowa*128;
                                    tymczasowa = (player_hp[(sockets[sockets_index])]/128);
                                    buffer2[8]= 128+tymczasowa;   		//HP postaci
                                    buffer2[9]= 128+(player_hp[(sockets[sockets_index])])-tymczasowa*128;
                                    tymczasowa = (player_mp[(sockets[sockets_index])]/128);
                                    buffer2[10]= 128+tymczasowa;   		//MP postaci
                                    buffer2[11]= 128+(player_mp[(sockets[sockets_index])])-tymczasowa*128;
                                    tymczasowa = (player_hp_max[(sockets[sockets_index])]/128);
                                    buffer2[12]= 128+tymczasowa;   		//HP MAX postaci
                                    buffer2[13]= 128+(player_hp_max[(sockets[sockets_index])])-tymczasowa*128;
                                    tymczasowa = (player_mp_max[(sockets[sockets_index])]/128);
                                    buffer2[14]= 128+tymczasowa;   		//MP MAX postaci
                                    buffer2[15]= 128+(player_mp_max[(sockets[sockets_index])])-tymczasowa*128;
                                    tymczasowa = (player_exp[(sockets[sockets_index])]/128);
                                    buffer2[16]= 128+tymczasowa;   		//EXP postaci
                                    buffer2[17]= 128+(player_exp[(sockets[sockets_index])])-tymczasowa*128;
                                    tymczasowa = (player_lvl[(sockets[sockets_index])]/128);
                                    buffer2[18]= 128+tymczasowa;   		//MP postaci
                                    buffer2[19]= 128+(player_lvl[(sockets[sockets_index])])-tymczasowa*128;
                                    tymczasowa = ((sockets[sockets_index]/128)/128);
                                    tymczasowa2 = sockets[sockets_index]/128;
                                    buffer2[20]= 128+tymczasowa;   		//socket logujacego sie gracza na serwerze
                                    buffer2[21]= 128+tymczasowa2-(tymczasowa*128);
                                    buffer2[22]= 128+(sockets[sockets_index])-(tymczasowa2*128)-(tymczasowa*128*128);
                                    buffer2[23]= 13;
                                    buffer2[24]= 10;
                                    result_s = send(sockets[sockets_index], buffer2, 25, 0);
                                    //printf("sending welcome to socket: %d result %d\n",sockets[sockets_index],result_s);
                                    spelle_l[sockets_index]=0; //zerowanie spelli
                                    for(i=0; i<player_buff_index[sockets[sockets_index]]; i++) {
                                        buffer2[0]=66; //B (66) - informacje o buffie
                                        tymczasowa = player_buff[sockets[index]][i]/128;
                                        buffer2[1]= 128+tymczasowa;
                                        buffer2[2]= 128+(player_buff[sockets[index]][i])-(tymczasowa*128);
                                        tymczasowa = (player_buff_time[sockets[index]][i]-cycle)/128;
                                        buffer2[3]= 128+tymczasowa;
                                        buffer2[4]= 128+(player_buff_time[sockets[index]][i]-cycle)-(tymczasowa*128);
                                        buffer2[5]= 129;
                                        buffer2[6]= 13;
                                        buffer2[7]= 10;
                                        result_s = send(sockets[index], buffer2, 8, 0);
                                    }
                                    sockets_index++;
                                }
                                else {
                                    //WYLOGOWANIE KLIENTA - tu nie trzeba nic wysylac bo klient w zasadzie nei jest zalogowany, mozna pomyslec o info dla klienta o nieprawidlowym loginie
                                    sockets_waiting_index--;
                                    for(i=index; i<sockets_waiting_index; i++) {
                                        sockets_waiting[i]=sockets_waiting[i+1];
                                    }
                                    //KONIEC WYLOGOWYWANIA
                                }
                            }
                        }
                    }
                }
            }
        }

        if (getTime() < endwait) {
            Sleep(1);
            kontrola_predkosci=4;
        }
        else
        {

            if (kontrola_predkosci<4)
                printf("Server is working too slowly! Making an attemp to cath up.\n");
            if (kontrola_predkosci==0) {
                printf("Server is working VERY slowly, resetting counter.\n");
                endwait = getTime();
            }
            kontrola_predkosci--;
            endwait = endwait + 500;

            for(m=1; m<(il_map+1); m++) { // NPC
                for(i=0; i<npc_max[m]; i++) {
                    if(npc_posn[m][i]>1) {
                        int npc_temp_x=npc_pos_x[m][i]/klocki;
                        int npc_temp_y=npc_pos_y[m][i]/klocki;
                        for(j=0; j<1; j++) { //zeby bylo co brejkowac
                            int przystanek = npc_droga[m][i];
                            if(npc_pos_x[m][i]>npc_pos[m][i][przystanek][0]) {
                                npc_pos_x[m][i] -= npc_speed[m][i][przystanek];
                                break;
                            }
                            if(npc_pos_x[m][i]<npc_pos[m][i][przystanek][0]) {
                                npc_pos_x[m][i] += npc_speed[m][i][przystanek];
                                break;
                            }
                            if(npc_pos_y[m][i]>npc_pos[m][i][przystanek][1]) {
                                npc_pos_y[m][i] -= npc_speed[m][i][przystanek];
                                break;
                            }
                            if(npc_pos_y[m][i]<npc_pos[m][i][przystanek][1]) {
                                npc_pos_y[m][i] += npc_speed[m][i][przystanek];
                                break;
                            }
                            npc_droga[m][i]++;
                            if((npc_droga[m][i]+1)>npc_posn[m][i])
                                npc_droga[m][i]=0;
                        }
                        int npc_temp2_x=npc_pos_x[m][i]/klocki;
                        int npc_temp2_y=npc_pos_y[m][i]/klocki;
                        if( (npc_temp2_x!=npc_temp_x) || (npc_temp2_y!=npc_temp_x) ) {
                            for(stan_i=0; stan_i<sockets_index; stan_i++)
                                if(npc_stan[m][i][stan_i]>0)
                                    npc_stan[m][i][stan_i]=1;
                        }

                    }
                    //printf("nr: %d   x/y: %d %d   bxby: %d %d\n", i, npc_pos_x[m][i], npc_pos_y[m][i], npc_pos_x[m][i]/16, npc_pos_y[m][i]/16);

                    buffer2[0]= 80;  //P - postac
                    buffer2[1]= 128+(i/128);   //numer postaci max 128*128 (16k)
                    buffer2[2]= 128+i-(i/128);
                    buffer2[3]= 81;  //R - ruch Q - nick

                    //npc_pos_x[1][i]   x
                    //npc_pos_y[1][i]   y
                    tymczasowa = ((npc_pos_x[m][i]/klocki)/128);
                    buffer2[4]= 128+tymczasowa;   		//X docelowe postaci
                    buffer2[5]= 128+(npc_pos_x[m][i]/klocki)-tymczasowa*128;

                    tymczasowa = (npc_pos_y[m][i]/128)/klocki;
                    buffer2[6]= 128+tymczasowa;   		//Y docelowe postaci
                    buffer2[7]= 128+(npc_pos_y[m][i]/klocki)-tymczasowa*128;

                    buffer2[8]= 128+npc_gfx[m][i];
                    buffer2[9]= 128;
                    for (j=0; j<10; j++) {
                        if(npc_nick[m][i][j]!=32)
                            buffer2[9]++;
                    }

                    for(int temp01=0; temp01<10; temp01++) {
                        buffer2[10+temp01]=npc_nick[m][i][temp01];
                    }
                    buffer2[20]= 13;
                    buffer2[21]= 10;

                    result_s = -666;
                    //if(odpowiedz==1) {
                    for (j=0; j<sockets_index; j++) {
                        if( (odpowiedz[(sockets[j])]==1) && (player_map[sockets[j]]==m)) {
                            tymczasowa=cycle/24;
                            if( ( ((player_x[sockets[j]]-((player_ekran_x[sockets[j]]/2)+1)) <= (npc_pos_x[m][i]/klocki)) && ((player_x[sockets[j]]+((player_ekran_x[sockets[j]]/2)+1)) >= (npc_pos_x[m][i]/klocki)) && ((player_y[sockets[j]]-((player_ekran_y[sockets[j]]/2)+1)) <= (npc_pos_y[m][i]/klocki)) && ((player_y[sockets[j]]+((player_ekran_y[sockets[j]]/2)+1)) >= (npc_pos_y[m][i]/klocki)) ) || (cycle==(tymczasowa*24)) ) {
                                switch(npc_stan[m][i][j]) {
                                case 0:
                                    result_s = send(sockets[j], buffer2, 22, 0);
                                    bytes_sent[(sockets[j])] += 22;
                                    npc_stan[m][i][j]=2;
                                    break;
                                case 1:
                                    for(stan_i=0; stan_i<9; stan_i++) buffer3[stan_i]=buffer2[stan_i];
                                    buffer3[3]=82;
                                    buffer3[9]=13;
                                    buffer3[10]=10;
                                    result_s = send(sockets[j], buffer3, 11, 0);
                                    bytes_sent[(sockets[j])] += 11;
                                    npc_stan[m][i][j]=2;
                                    break;
                                }
                            }
                        }
                    }

                }

                for(i=0; i<mob_max[m]; i++) { //mobki
                    if(mob_posn[m][i]>1) {
                        if(mob_hp[m][i]>0) {
                            if(mob_atak[m][i]>0) mob_atak[m][i]=mob_atak[m][i]-cycleX;
                            int mob_temp_x=mob_pos_x[m][i]/klocki;
                            int mob_temp_y=mob_pos_y[m][i]/klocki;
                            if(mob_target[m][i]==-1) {
                                for(j=0; j<1; j++) { //zeby bylo co brejkowac
                                    int przystanek = mob_droga[m][i];
                                    if(mob_pos_x[m][i]>mob_pos[m][i][przystanek][0]) {
                                        mob_pos_x[m][i] -= mob_speed[m][i][przystanek];
                                        break;
                                    }
                                    if(mob_pos_x[m][i]<mob_pos[m][i][przystanek][0]) {
                                        mob_pos_x[m][i] += mob_speed[m][i][przystanek];
                                        break;
                                    }
                                    if(mob_pos_y[m][i]>mob_pos[m][i][przystanek][1]) {
                                        mob_pos_y[m][i] -= mob_speed[m][i][przystanek];
                                        break;
                                    }
                                    if(mob_pos_y[m][i]<mob_pos[m][i][przystanek][1]) {
                                        mob_pos_y[m][i] += mob_speed[m][i][przystanek];
                                        break;
                                    }
                                    mob_droga[m][i]++;
                                    if((mob_droga[m][i]+1)>mob_posn[m][i])
                                        mob_droga[m][i]=0;
                                }
                            }
                            else {
                                for(j=0; j<1; j++) { //zeby bylo co brejkowac
                                    int przystanek = mob_droga[m][i];
                                    if(mob_pos_x[m][i]>(player_x[mob_target[m][i]]*klocki)+klocki) {
                                        mob_pos_x[m][i] -= mob_speed[m][i][przystanek];
                                        break;
                                    }
                                    if(mob_pos_x[m][i]<(player_x[mob_target[m][i]]*klocki)-klocki) {
                                        mob_pos_x[m][i] += mob_speed[m][i][przystanek];
                                        break;
                                    }
                                    if(mob_pos_y[m][i]>(player_y[mob_target[m][i]]*klocki)+klocki) {
                                        mob_pos_y[m][i] -= mob_speed[m][i][przystanek];
                                        break;
                                    }
                                    if(mob_pos_y[m][i]<(player_y[mob_target[m][i]]*klocki)-klocki) {
                                        mob_pos_y[m][i] += mob_speed[m][i][przystanek];
                                        break;
                                    }
                                    if((mob_atak[m][i]==0)) {
                                        for(stan_i=0; stan_i<sockets_index; stan_i++) { //znajdz numer gracza
                                            if(sockets[stan_i]==mob_target[m][i]) break;
                                        }
                                        player_ruch[mob_target[m][i]]=1;  //przerywanie regenu gracza/itp
                                        printf("%d :",player_hp[mob_target[m][i]]);
                                        player_hp[mob_target[m][i]]-=wylicz_hit_moba(mob_moc_atak1[m][i], mob_moc_atak2[m][i]);
                                        printf("%d \n",player_hp[mob_target[m][i]]);
                                        mob_atak[m][i]=mob_cz_atak[m][i];
                                        for(stan_m=0; stan_m<sockets_index; stan_m++)
                                            player_stan[sockets[stan_i]][stan_m]=0;
                                        if(player_hp[mob_target[m][i]]<1) {
                                            player_hp[mob_target[m][i]]=0;
                                            mob_target[m][i]=-1; //mob zostawia gracza w spokoju
                                            //gracz ginie
                                        }
                                    }
                                    //wylicz_hit_mob(mob_moc_atak1, mob_moc_atak2);
                                    //zadawanie DMG
                                }
                            }
                            int mob_temp2_x=mob_pos_x[m][i]/klocki;
                            int mob_temp2_y=mob_pos_y[m][i]/klocki;
                            if( (mob_temp2_x!=mob_temp_x) || (mob_temp2_y!=mob_temp_x) ) {
                                for(stan_i=0; stan_i<sockets_index; stan_i++)
                                    if(mob_stan[m][i][stan_i]>0)
                                        mob_stan[m][i][stan_i]=1;
                            }
                        } else {
                            if(mob_dead[m][i]>0) {
                                mob_dead[m][i]--;
                                if(mob_dead[m][i]==1) {
                                    mob_target[m][i]=-1;
                                    mob_dead[m][i]=0;
                                    mob_hp[m][i]=mob_max_hp[m][i];
                                    mob_pos_x[m][i]=mob_pos[m][i][0][0];
                                    mob_pos_y[m][i]=mob_pos[m][i][0][1];
                                    for(stan_i=0; stan_i<sockets_index; stan_i++)
                                        mob_stan[m][i][stan_i]=0;
                                }
                            } else {
                                mob_dead[m][i]=(respawn_1);
                            }
                        }
                    }
                    //printf("nr: %d   x/y: %d %d   bxby: %d %d\n", i, mob_pos_x[m][i], mob_pos_y[m][i], mob_pos_x[m][i]/16, mob_pos_y[m][i]/16);

                    buffer2[0]= 80;  //P - postac
                    buffer2[1]= 128+64+(i/128);   //numer postaci max 128*128 (16k) od 8192 sa moby
                    buffer2[2]= 128+i-(i/128);
                    buffer2[3]= 81;  //R - ruch Q - nick

                    //mob_pos_x[1][i]   x
                    //mob_pos_y[1][i]   y
                    tymczasowa = ((mob_pos_x[m][i]/klocki)/128);
                    buffer2[4]= 128+tymczasowa;   		//X docelowe postaci
                    buffer2[5]= 128+(mob_pos_x[m][i]/klocki)-tymczasowa*128;

                    tymczasowa = (mob_pos_y[m][i]/128)/klocki;
                    buffer2[6]= 128+tymczasowa;   		//Y docelowe postaci
                    buffer2[7]= 128+(mob_pos_y[m][i]/klocki)-tymczasowa*128;

                    buffer2[8]= 128+mob_gfx[m][i];
                    buffer2[9]= 128;
                    for (j=0; j<10; j++) {
                        if(mob_nick[m][i][j]!=32)
                            buffer2[9]++;
                    }

                    for(int temp01=0; temp01<10; temp01++) {
                        buffer2[10+temp01]=mob_nick[m][i][temp01];
                    }
                    tymczasowa = ((mob_lvl[m][i])/128);
                    buffer2[20]= 128+tymczasowa;   		//lvl moba
                    buffer2[21]= 128+(mob_lvl[m][i])-tymczasowa*128;
                    tymczasowa = ((mob_hp[m][i])/128);
                    buffer2[22]= 128+tymczasowa;   		//hp moba
                    buffer2[23]= 128+(mob_hp[m][i])-tymczasowa*128;
                    tymczasowa = ((mob_max_hp[m][i])/128);
                    buffer2[24]= 128+tymczasowa;   		//max hp moba
                    buffer2[25]= 128+(mob_max_hp[m][i])-tymczasowa*128;
                    tymczasowa = ((mob_mp[m][i])/128);
                    buffer2[26]= 128+tymczasowa;   		//mp moba
                    buffer2[27]= 128+(mob_mp[m][i])-tymczasowa*128;
                    tymczasowa = ((mob_max_mp[m][i])/128);
                    buffer2[28]= 128+tymczasowa;   		//max mp moba
                    buffer2[29]= 128+(mob_max_mp[m][i])-tymczasowa*128;
                    buffer2[30]= 13;
                    buffer2[31]= 10;

                    result_s = -666;
                    //if(odpowiedz==1) {
                    for (j=0; j<sockets_index; j++) {
                        if( (odpowiedz[(sockets[j])]==1) && (player_map[sockets[j]]==m) ) {
                            tymczasowa=cycle/24;
                            if( ( ((player_x[sockets[j]]-((player_ekran_x[sockets[j]]/2)+1)) <= (mob_pos_x[m][i]/klocki)) && ((player_x[sockets[j]]+((player_ekran_x[sockets[j]]/2)+1)) >= (mob_pos_x[m][i]/klocki)) && ((player_y[sockets[j]]-((player_ekran_y[sockets[j]]/2)+1)) <= (mob_pos_y[m][i]/klocki)) && ((player_y[sockets[j]]+((player_ekran_y[sockets[j]]/2)+1)) >= (mob_pos_y[m][i]/klocki)) ) || (cycle==(tymczasowa*24)) ) {
                                switch(mob_stan[m][i][j]) {
                                case 0:
                                    result_s = send(sockets[j], buffer2, 32, 0);
                                    bytes_sent[(sockets[j])] += 32;
                                    mob_stan[m][i][j]=2;
                                    break;
                                case 1:
                                    for(stan_i=0; stan_i<9; stan_i++) buffer3[stan_i]=buffer2[stan_i];
                                    buffer3[3]=82;
                                    buffer3[9]=13;
                                    buffer3[10]=10;
                                    result_s = send(sockets[j], buffer3, 11, 0);
                                    bytes_sent[(sockets[j])] += 11;
                                    mob_stan[m][i][j]=2;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            for(i=0; i<sockets_index; i++) { // GRACZE
                //printf("     5> %d\n",player_y[(sockets[2])]);
                buffer2[0]= 80;  //P - postac
                tymczasowa=(sockets[i]/128);
                buffer2[1]= 128+32+tymczasowa;   //numer postaci max 128*128 (16k)
                buffer2[2]= 128+sockets[i]-(tymczasowa*128);
                //printf("     6> %d\n",player_y[(sockets[2])]);

                buffer2[3]= 81;  //R - ruch Q - nick

                //npc_pos_x[1][i]   x
                //npc_pos_y[1][i]   y
                //printf("\nDp%d / %d : %d %d\n",i,(sockets[i]),(player_x[(sockets[i])]),(player_y[(sockets[i])]));
                //printf("p%d : %d %d\n",sockets[i],(player_x[(sockets[i])]),(player_y[(sockets[i])]));
                tymczasowa = ((player_x[(sockets[i])])/128);
                buffer2[4]= 128+tymczasowa;   		//X docelowe postaci
                buffer2[5]= 128+((player_x[sockets[i]]))-tymczasowa*128;

                tymczasowa = ((player_y[sockets[i]])/128);
                buffer2[6]= 128+tymczasowa;   		//Y docelowe postaci
                buffer2[7]= 128+(player_y[sockets[i]])-tymczasowa*128;

                buffer2[8]= 128+1;
                buffer2[9]= 128;
                for(j=0; j<10; j++) {
                    if(login[nr_konta[sockets[i]]][j]==95) //95 to _
                        buffer2[10+j] = 0;  //
                    else {
                        buffer2[10+j] = login[nr_konta[sockets[i]]][j];
                        buffer2[9]++;
                    }
                }
                //login[nr_konta[sockets[sockets_index]]]

                tymczasowa = ((player_lvl[sockets[i]])/128);
                buffer2[20]= 128+tymczasowa;   		//lvl gracza
                buffer2[21]= 128+(player_lvl[sockets[i]])-tymczasowa*128;
                tymczasowa = ((player_hp[sockets[i]])/128);
                buffer2[22]= 128+tymczasowa;   		//hp gracza
                buffer2[23]= 128+(player_hp[sockets[i]])-tymczasowa*128;
                tymczasowa = ((player_hp_max[sockets[i]])/128);
                buffer2[24]= 128+tymczasowa;   		//max hp gracza
                buffer2[25]= 128+(player_hp_max[sockets[i]])-tymczasowa*128;
                tymczasowa = ((player_mp[sockets[i]])/128);
                buffer2[26]= 128+tymczasowa;   		//mp gracza
                buffer2[27]= 128+(player_mp[sockets[i]])-tymczasowa*128;
                tymczasowa = ((player_mp_max[sockets[i]])/128);
                buffer2[28]= 128+tymczasowa;   		//max mp gracza
                buffer2[29]= 128+(player_mp_max[sockets[i]])-tymczasowa*128;
                buffer2[30]= 13;
                buffer2[31]= 10;
                result_s = -666;
//					if(odpowiedz==1) {
                for (j=0; j<sockets_index; j++) {
                    if( (sockets[j]!=sockets[i]) && (player_map[sockets[j]]==player_map[sockets[i]]) && (player_stan[sockets[i]][j]<2) ) {
                        tymczasowa=cycle/8;
                        if( ( ((player_x[sockets[j]]-((player_ekran_x[sockets[j]]/2)+5)) <= (player_x[(sockets[i])])) && ((player_x[sockets[j]]+((player_ekran_x[sockets[j]]/2)+5)) >= (player_x[(sockets[i])])) && ((player_y[sockets[j]]-((player_ekran_y[sockets[j]]/2)+5)) <= (player_y[(sockets[i])])) && ((player_y[sockets[j]]+((player_ekran_y[sockets[j]]/2)+5)) >= (player_y[(sockets[i])])) ) || (cycle==(tymczasowa*8)) )  {
                            if(odpowiedz[(sockets[j])]==1) {
                                result_s = send(sockets[j], buffer2, 32, 0);
                                bytes_sent[(sockets[j])] += 32;
                                player_stan[sockets[i]][j]=2;
                                //	printf("%d %d:%d %d,%d %d\n",buffer2[1]+128,buffer2[2]+128,buffer2[4]+128,buffer2[5]+128,buffer2[6]+128,buffer2[7]+128);
                            }
                        }
                    }
                }
            }
            for(i=0; i<sockets_index; i++) { //jesli gracz jest na okreslonym polu zmien mape
                j=teleport(player_x[sockets[i]], player_y[sockets[i]], player_map[sockets[i]], sockets[i]);
                if(j>0) {
                    player_x[sockets[i]]=teleport_x; //zeby uniknac podwojnej teleportacji
                    player_y[sockets[i]]=teleport_y; //klient podsy�a swoje koordy pozniej i server zdazy go tepnac jesli koordy teleportow na mapach sobie odpowiadaja
                    if(player_revive[sockets[i]]==1) {
                        buffer2[0]= 82;  //R - revive
                        tymczasowa=(j/128);
                        buffer2[1]= 128+tymczasowa;   //numer nowej mapy
                        buffer2[2]= 128+j-(tymczasowa*128);
                        tymczasowa=(teleport_x/128);
                        buffer2[3]= 128+tymczasowa;   //numer nowej mapy
                        buffer2[4]= 128+teleport_x-(tymczasowa*128);
                        tymczasowa=(teleport_y/128);
                        buffer2[5]= 128+tymczasowa;   //numer nowej mapy
                        buffer2[6]= 128+teleport_y-(tymczasowa*128);
                        buffer2[7]= 13;
                        buffer2[8]= 10;
                        result_s = -666;
                        result_s = send(sockets[i], buffer2, 9, 0);
                        player_revive[sockets[i]]=0;
                        player_hp[sockets[i]]=player_hp_max[sockets[i]]/5;
                        player_mp[sockets[i]]=player_mp_max[sockets[i]]/5;
                    }
                    else {
                        buffer2[0]= 77;  //M - zmiana mapy
                        tymczasowa=(j/128);
                        buffer2[1]= 128+tymczasowa;   //numer nowej mapy
                        buffer2[2]= 128+j-(tymczasowa*128);
                        tymczasowa=(teleport_x/128);
                        buffer2[3]= 128+tymczasowa;   //numer nowej mapy
                        buffer2[4]= 128+teleport_x-(tymczasowa*128);
                        tymczasowa=(teleport_y/128);
                        buffer2[5]= 128+tymczasowa;   //numer nowej mapy
                        buffer2[6]= 128+teleport_y-(tymczasowa*128);
                        buffer2[7]= 13;
                        buffer2[8]= 10;
                        result_s = -666;
                        result_s = send(sockets[i], buffer2, 9, 0);
                        printf("wyslalem zwrot!\n");
                    }
                    for(stan_m=1; stan_m<(il_map+1); stan_m++) { //wszystko co sie rusza nalezy wyslac temu graczowi (zmiana mapy to nie pikus)
                        for(stan_i=0; stan_i<npc_max[stan_m]; stan_i++)
                            npc_stan[stan_m][stan_i][i]=0;
                        for(stan_i=0; stan_i<mob_max[stan_m]; stan_i++)
                            mob_stan[stan_m][stan_i][i]=0;
                    }
                    for(stan_m=1; stan_m<(il_map+1); stan_m++) { //jesli jakis mob ma go w targecie wyczysc
                        for(stan_i=0; stan_i<mob_max[stan_m]; stan_i++) {
                            if(mob_target[stan_m][stan_i]==sockets[i])
                                mob_target[stan_m][stan_i]=-1;
                        }
                    }
                    for(stan_i=0; stan_i<sockets_index; stan_i++)
                        player_stan[sockets[stan_i]][i]=0;
                    for(index = 0; index<sockets_index ; index++) { //usuwanie graczy z mapy ktora opuszcza dany gracz
                        buffer2[0]= 80;  //P - postac
                        tymczasowa=(sockets[index]/128);
                        buffer2[1]= 128+64+tymczasowa;
                        buffer2[2]= 128+sockets[index]-(tymczasowa*128);
                        buffer2[3]= 68;  //R - ruch Q - nick D (68) - usuniecie
                        buffer2[4]= 128;
                        buffer2[5]= 127;
                        buffer2[6]= 128;
                        buffer2[7]= 127;
                        buffer2[8]= 128+1;
                        buffer2[9]= 128;
                        for(n=0; n<10; n++)
                            buffer2[10+n]= 32;
                        buffer2[20]= 13;
                        buffer2[21]= 10;
                        result_s = -666;
                        if((sockets[i]!=sockets[index]) && (player_map[sockets[i]]==player_map[sockets[index]]) ) { //usuwaj wszystkich graczy poza nim samym (1 warunek) i tymi ktorzy nie sa na jego mapie (2 warunek)
                            result_s = send(sockets[i], buffer2, 22, 0);
                            bytes_sent[(sockets[i])] += 22;
                        }
                    }
                    for(index = 0; index<sockets_index ; index++) { //usuwanie gracza u innych graczy na tej mapie
                        buffer2[0]= 80;  //P - postac
                        tymczasowa=(sockets[i]/128);
                        buffer2[1]= 128+32+tymczasowa;
                        buffer2[2]= 128+sockets[i]-(tymczasowa*128);
                        buffer2[3]= 68;  //R - ruch Q - nick D (68) - usuniecie
                        buffer2[4]= 128;
                        buffer2[5]= 127;
                        buffer2[6]= 128;
                        buffer2[7]= 127;
                        buffer2[8]= 128+1;
                        buffer2[9]= 128;
                        for(n=0; n<10; n++)
                            buffer2[10+n]= 32;
                        buffer2[20]= 13;
                        buffer2[21]= 10;
                        result_s = -666;
                        if( (sockets[i]!=sockets[index]) && (player_map[sockets[i]]==player_map[sockets[index]]) ) { //wysylaj do wszystkich graczy usuniecie go, poza nim samym (1 warunek) i tymi ktorzy nie sa na jego mapie (2 warunek)
                            result_s = send(sockets[index], buffer2, 22, 0);
                            bytes_sent[(sockets[index])] += 22;
                        }
                    }
                    for(index=0; index<npc_max[player_map[sockets[i]]]; index++) { //usuwanie npc graczowi sockets[i] z mapy ktora opuszcza

                        buffer2[0]= 80;  //P - postac
                        tymczasowa=(index/128);
                        buffer2[1]= 128+tymczasowa;
                        buffer2[2]= 128+index-(tymczasowa*128);
                        buffer2[3]= 68;  //R - ruch Q - nick D (68) - usuniecie
                        buffer2[4]= 128;
                        buffer2[5]= 127;
                        buffer2[6]= 128;
                        buffer2[7]= 127;
                        buffer2[8]= 128+1;
                        buffer2[9]= 128;
                        for(n=0; n<10; n++)
                            buffer2[10+n]= 32;
                        buffer2[20]= 13;
                        buffer2[21]= 10;
                        result_s = -666;
                        result_s = send(sockets[i], buffer2, 22, 0);
                        bytes_sent[(sockets[i])] += 22;
                    }
                    for(index=0; index<mob_max[player_map[sockets[i]]]; index++) { //usuwanie npc graczowi sockets[i] z mapy ktora opuszcza

                        buffer2[0]= 80;  //P - postac
                        tymczasowa=(index/128);
                        buffer2[1]= 128+64+tymczasowa;
                        buffer2[2]= 128+index-(tymczasowa*128);
                        buffer2[3]= 68;  //R - ruch Q - nick D (68) - usuniecie
                        buffer2[4]= 128;
                        buffer2[5]= 127;
                        buffer2[6]= 128;
                        buffer2[7]= 127;
                        buffer2[8]= 128+1;
                        buffer2[9]= 128;
                        for(n=0; n<10; n++)
                            buffer2[10+n]= 32;
                        buffer2[20]= 13;
                        buffer2[21]= 10;
                        result_s = -666;
                        result_s = send(sockets[i], buffer2, 22, 0);
                        bytes_sent[(sockets[i])] += 22;
                    }
                    player_map[sockets[i]] = j;
                }
            }
            for(j=0; j<sockets_index; j++) {
                odpowiedz[(sockets[j])]--;
                if(player_czas_cast[sockets[j]]>0)
                    player_czas_cast[sockets[j]]--;
            }
            for(j=0; j<sockets_waiting_index; j++)
                odpowiedz[(sockets_waiting[j])]--;
            cycle++;
            int run_day=cycle/((24*60*60)*2);
            int run_hou=(cycle/((60*60)*2))-(run_day*24);
            int run_min=(cycle/((60)*2))-(run_day*24*60)-(run_hou*60);
            int run_sec=(cycle/2)-(run_day*24*60*60)-(run_hou*60*60)-(run_min*60);
            printf("czas uruchomienia: %dd %dh %dm %ds   ILOSC POLACZEN: %d   ODP: %d\n", run_day, run_hou, run_min, run_sec, sockets_index, odpowiedz[sockets[0]]);
        }

    }

    for (result = 0; result < sockets_index; result++) {
        closesocket(sockets[sockets_index]);
    }

    closesocket(descriptor);
#ifdef WIN32
    WSACleanup();
#endif

    return (0);
}
