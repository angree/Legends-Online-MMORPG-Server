
long getTime() // in ms
{

    timeval t;
    gettimeofday(&t, NULL);

    return t.tv_sec * 1000 + t.tv_usec / 1000;

}

int sprawdz_haslo(char bufor[256]) {
    int wynik = -1;
    int przes = 0;
    int przes_h = 0;
    int przes_k = 0;
    int login_l = 0;
    int pass_l = 0;
    int z = 0;
    char login_temp[10];
    char pass_temp[10];
    for(z=0; z<256; z++) {
        if(bufor[z]==76) {
            przes=z+2;
            break;
        }
    }
    for(z=0; z<256; z++) {
        if(bufor[z]==58) {
            przes_h=z;
            break;
        }
    }
    for(z=0; z<256; z++) {
        if(bufor[z]==0) {
            przes_k=z;
            break;
        }
    }
    login_l=przes_h-przes-1;
    pass_l=przes_k-przes_h-1;
    for(int j=0; j<10; j++) {
        if(j<login_l)
            login_temp[j] = bufor[przes+1+j];
        else
            login_temp[j] = 95;
    }
    for(int j=0; j<10; j++) {
        if(j<pass_l)
            pass_temp[j] = bufor[przes_h+1+j];
        else
            pass_temp[j] = 95;
    }
    for(int i=0; i<ilosc_kont; i++) {
        z=1; //zgodnosc = 1
        for(int j=0; j<10; j++) {
            if( login[i][j]!=login_temp[j] )  //95
                z=0;
            if( pass[i][j]!=pass_temp[j] )
                z=0;
            //printf("pass: %d != %d\n", pass[i][j], bufor[przes_h+1+j]);
        }
        if (z==1) {
            if(wynik!=-1)	printf("Error! Found acc twice!\n");
            wynik=i;
        }
    }
    printf("wynik: %d\n", wynik);
    return wynik;
}
//TU DOKONYWAC ZMIAN JAK DODAJE SIE MAPY

int teleport(int posx, int posy, int posm, int id_65) {
    int zwrot = 0;
    if(player_revive[id_65]>0) { //jesli sie podnosi po deadzie
        switch(posm) {
        case 1:
            zwrot = 1;
            teleport_x = 36;
            teleport_y=6;
            break;
        case 2:
            zwrot = 1;
            teleport_x = 36;
            teleport_y=6;
            break;	//jesli ginie na planszy 2 to wroc go do 1
        case 3:
            zwrot = 1;
            teleport_x = 36;
            teleport_y=6;
            break;	//jesli ginie na planszy 3 to wroc go do 1
        case 4:
            zwrot = 1;
            teleport_x = 36;
            teleport_y=6;
            break;	//jesli ginie na planszy 4 to wroc go do 1
        case 5:
            zwrot = 1;
            teleport_x = 36;
            teleport_y=6;
            break;	//jesli ginie na planszy 5 to wroc go do 1
        }
    }
    else {
        switch(posm) {
        case 1:
            if( (posx==25) && (posy==1) ) {
                zwrot = 2;
                teleport_x = 3;
                teleport_y=4;
            }
            if( (posx==1) && (posy==38) ) {
                zwrot = 3;
                teleport_x = 1;
                teleport_y=2;
            }
            if( (posx==63) && (posy==38) ) {
                zwrot = 3;
                teleport_x = 67;
                teleport_y=2;
            }
            if( (posx==25) && (posy==3) ) {
                zwrot = 5;
                teleport_x = 3;
                teleport_y=4;
            }
            break;
        case 2:
            if( (posx==2) && (posy==4) ) {
                zwrot = 1;
                teleport_x =26;
                teleport_y=1;
            }
            break;
        case 3:
            if( (posx==1) && (posy==1) ) {
                zwrot = 1;
                teleport_x=1;
                teleport_y=37;
            }
            if( (posx==67) && (posy==1) ) {
                zwrot = 1;
                teleport_x = 62;
                teleport_y=38;
            }
            if( (posx==1) && (posy==18) ) {
                zwrot = 4;
                teleport_x = 1;
                teleport_y=2;
            }
            break;
        case 4:
            if( (posx==1) && (posy==1) ) {
                zwrot = 3;
                teleport_x=1;
                teleport_y=17;
            }
            break;
        case 5:
            if( (posx==2) && (posy==4) ) {
                zwrot = 1;
                teleport_x=26;
                teleport_y=3;
            }
            break;
        }
    }
    return zwrot;
}

int revive(int nr_mapy, int x_y) {
    int zwrot = -1;
    int new_x=0;
    int new_y=0;
    switch(nr_mapy) {
    case 1:
        new_x=2;
        new_y=2;
        break;
    case 2:
        new_x=2;
        new_y=2;
        break;
    }
    switch(x_y) {
    case 0:
        zwrot=new_x;
        break;
    case 1:
        zwrot=new_y;
        break;
    }
    return zwrot;
}


int random (int max) {
    int zwrot=0;
    zwrot = (rand() % max);
    return zwrot;
}

int odleglosc(int odleglosc_x, int odleglosc_y, int odleglosc_c) {
    int zwrot = 0;
    if ( ((odleglosc_x*odleglosc_x)+(odleglosc_x*odleglosc_x))<=(odleglosc_c*odleglosc_c) ) {
        zwrot=1;
    }
    return zwrot;
}

void wylicz_staty(int nr_klasy, int lvl_postaci, int temp_sock_index) {  //0-priest
    int temp_i = 0;
    w_spirit = 0;
    w_intelect = 0;
    w_stamina = 0;
    w_strength = 0;
    w_agility = 0;
    switch(nr_klasy) {
    case 0:
        w_spirit = 10+(((lvl_postaci+10)*(lvl_postaci+10))/10);  //priest
        w_intelect = 6+(((lvl_postaci+7)*(lvl_postaci+7))/10);
        w_stamina = 2+(((lvl_postaci+4)*(lvl_postaci+4))/10);
        w_strength = 1+lvl_postaci;
        w_agility = 1+lvl_postaci;
        break;
    case 1:
        w_spirit = 8+(((lvl_postaci+6)*(lvl_postaci+6))/10);   //mage
        w_intelect = 0+(((lvl_postaci+11)*(lvl_postaci+11))/10);
        w_stamina = 3+(((lvl_postaci+2)*(lvl_postaci+2))/10);
        w_strength = 1+lvl_postaci;
        w_agility = 1+lvl_postaci;
        break;
    }
    if(temp_sock_index!=(-1)) {
        for(temp_i=0; temp_i<30; temp_i++) {
            if(player_equip[temp_sock_index][temp_i]!=0) {
                w_spirit += item_spi[player_equip[temp_sock_index][temp_i]];
                w_intelect += item_int[player_equip[temp_sock_index][temp_i]];
                w_stamina += item_sta[player_equip[temp_sock_index][temp_i]];
                w_strength += item_str[player_equip[temp_sock_index][temp_i]];
                w_agility += item_agi[player_equip[temp_sock_index][temp_i]];
            }
        }
        //item_sta[player_inventory[temp_sock_index][temp_i]]
    }
}

int wylicz_hit(int nr_spella,int lvl_spella) {
    int wynik = 0;
    int crit = 0;
    lvl_spella++;
    switch(nr_spella) {
    case 1:
        crit=random(100);
        if(crit<5) crit=8;
        else crit=5;
        wynik=(( (3*lvl_spella)+random(2*lvl_spella) )*crit)/5;
        break;
    case 102:
        crit=random(100);
        if(crit<5) crit=8;
        else crit=5;
        wynik=(( (10*lvl_spella)+random(4*lvl_spella) )*crit)/5;
        break;
    }
    return wynik;   // sp 1 - 1 6-10  2 9-15  3 12-20  4 15-25
}

int wylicz_hit_moba(int atak_podstawowy,int atak_bonusowy) {
    int wynik = 0;
    int crit = 0;
    crit=random(100);
    if(crit<5) crit=8;
    else crit=5;
    wynik=((atak_podstawowy+random(atak_bonusowy))*crit)/5;
    return wynik;
}

int casttime(int nr_spella) {
    int wynik = 5000;
    switch(nr_spella) {
    case 1:
        wynik=2500;
        break;
    case 102:
        wynik=3500;
        break;
    }
    return wynik;
}

int zasieg_spella(int nr_spella) {
    int wynik = 2;
    switch(nr_spella) {
    case 1:
        wynik=6;
        break;
    case 102:
        wynik=6;
        break;
    }
    return wynik;
}

int koszt_spella(int nr_spella, int lvl_spella) {
    int wynik = 300000;
    lvl_spella++;
    switch(nr_spella) {
    case 1:
        wynik=(5*lvl_spella)-(lvl_spella>>1)-(lvl_spella>>2)-(lvl_spella>>2)-(lvl_spella>>4)-(lvl_spella>>5);
        break;  //1- 13  2 - 20  3 - 26  4 - 33
    case 102:
        wynik=(11*lvl_spella)-(lvl_spella>>1)-(lvl_spella>>2)-(lvl_spella>>2)-(lvl_spella>>4)-(lvl_spella>>5);
        break;  //1- 13  2 - 20  3 - 26  4 - 33
    }
    return wynik;
}

int exp(int level)  {
    int zwrot = -1;
    int exp_temp = 15*(level/5);
    zwrot=23+exp_temp+(level*level*86);
    return zwrot;
}

int killexp(int level, int level_postaci)  {
    int zwrot = -1;
    int exp_temp2 = 100;
    int roznica = level_postaci-level;
    if(roznica>0) {
        exp_temp2=(125/(roznica+1));
    }
    if(roznica<-2) {
        level=level_postaci+2;
    }
    int exp_temp = (level/2)+1;
    zwrot=((15*exp_temp)*exp_temp2)/25;
    return zwrot;
}

int rip2_x(char bufor[256]) {
    int wynik = 0;
    int przes = 0;
    int z = 0;
    for(z=0; z<256; z++) {
        if(bufor[z]==67) {
            przes=z;
            break;
        }
    }
    wynik=wynik+(bufor[przes+1]-64)*64;
    wynik=wynik+(bufor[przes+2]-64);
    return wynik;
}
int rip2_y(char bufor[256]) {
    int wynik = 0;
    int przes = 0;
    int z = 0;
    for(z=0; z<256; z++) {
        if(bufor[z]==67) {
            przes=z;
            break;
        }
    }
    wynik=wynik+(bufor[przes+3]-64)*64;
    wynik=wynik+(bufor[przes+4]-64);
    return wynik;
}
int rip_x(char bufor[256]) {
    int wynik = 0;
    int przes = 0;
    int z = 0;
    for(z=0; z<256; z++) {
        if(bufor[z]==67) {
            przes=z;
            break;
        }
    }
    wynik=wynik+(bufor[przes+1]-48)*100;
    wynik=wynik+(bufor[przes+2]-48)*10;
    wynik=wynik+(bufor[przes+3]-48);
    return wynik;
}
int rip_y(char bufor[256]) {
    int wynik = 0;
    int przes = 0;
    int z = 0;
    for(z=0; z<256; z++) {
        if(bufor[z]==58) {
            przes=z;
            break;
        }
    }
    wynik=wynik+(bufor[przes+1]-48)*100;
    wynik=wynik+(bufor[przes+2]-48)*10;
    wynik=wynik+(bufor[przes+3]-48);
    return wynik;
}
void porzadkuj_inventory(int temp_sock_index) {
    int temp_i,temp_j;
    for(temp_i=0; temp_i<69; temp_i++) {
        if((player_inventory[temp_sock_index][temp_i]==0) && (player_inventory[temp_sock_index][temp_i+1]!=0)) {
            player_inventory[temp_sock_index][temp_i]=player_inventory[temp_sock_index][temp_i+1];
            player_inventory_ilosc[temp_sock_index][temp_i]=player_inventory_ilosc[temp_sock_index][temp_i+1];
            player_inventory[temp_sock_index][temp_i+1]=0;
            player_inventory_ilosc[temp_sock_index][temp_i+1]=0;

        }
    }
}
