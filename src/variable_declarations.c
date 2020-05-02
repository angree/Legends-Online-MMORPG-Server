
const int version = 25; //wersja

//maksymalna ilosc kont to 128k


#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

const int D_PORT = 2123;
#define D_HOST2 "185.157.80.235"
#define D_HOST3 "192.168.1.104"
const int D_QUEUE = 32;
const int D_SOCKETS = 600;
const int D_INFO = 256;
const int MAX_CHARS = 128;
const int MAX_MAPS = 128;
const int MAX_MOVE = 64;
const int MAX_LAG = 30;
const int il_map = 5;
const int klocki = 16;
const int mob_data_ilosc = (128*1024);
const int IL_QUESTOW = 65536;

int mob_data_max=0;
int mob_data_index=0;
char mob_data_nick[mob_data_ilosc][11];
int mob_data_gfx[mob_data_ilosc];
int mob_data_type[mob_data_ilosc];
int mob_data_cz_atak[mob_data_ilosc];
int mob_data_moc_atak1[mob_data_ilosc];
int mob_data_moc_atak2[mob_data_ilosc];
int mob_data_lvl[mob_data_ilosc];
int mob_data_max_hp[mob_data_ilosc];
int mob_data_max_mp[mob_data_ilosc];
int mob_data_loot_il[mob_data_ilosc];
int mob_data_loot_index[mob_data_ilosc][64];
int mob_data_loot_szansa[mob_data_ilosc][64];

int quest_lvl[IL_QUESTOW];		//lvl wymagany dla qesta. 0 - brak wymagan <0 - na przyszlosc (inne parametry, q klasowe, etc.)
char quest_oddaj[IL_QUESTOW][11];  	//nazwa npc ktory moze przyjac zakonczony quest
int quest_typ[IL_QUESTOW];		//1-zabijanie mobow
int quest_dozabicia[IL_QUESTOW];	//ilosc mobkow/przedmiotow do zebrania (ogolnie), jesli 0 to licza sie szczegolowe rzadania
int quest_moby_rodzaje[IL_QUESTOW];	//ilosc rodzajow mobow/przedmiotow ktore nalezy zebrac/zabic (max 8)
int quest_moby[IL_QUESTOW][8];		//id mobow/przedmiotow ktore nalezy zebrac/zabic (max 8)
int quest_moby_ilosc[IL_QUESTOW][8];	//ilosc poszczegolnych mobow/przedmiotow ktore nalezy zebrac/zabic


int ilosc_itemow = 0;
char item_nazwa[(128*1024)][31];  //ilosc itemow max - 128tys
int item_grafika[(128*1024)];
int item_typ[(128*1024)];
int item_wartosc[(128*1024)];
int item_dura[(128*1024)];
int item_stack[(128*1024)];

int item_arm[(128*1024)];
int item_md[(128*1024)];
int item_bd[(128*1024)];
int item_bh[(128*1024)];

int item_int[(128*1024)];
int item_spi[(128*1024)];
int item_sta[(128*1024)];
int item_agi[(128*1024)];
int item_str[(128*1024)];

char login[(128*1024)][11];
char pass[(128*1024)][11];
int konto_start[(128*1024)][3];
int konto_hp[(128*1024)];
int konto_mp[(128*1024)];
int konto_hp_max[(128*1024)];
int konto_mp_max[(128*1024)];
int konto_exp[(128*1024)];
int konto_lvl[(128*1024)];
int konto_klasa[(128*1024)];
int konto_spirit[(128*1024)];

int konto_equip[(128*1024)][30];

int konto_inventory[(128*1024)][75];
int konto_inventory_ilosc[(128*1024)][75];
int konto_bagi[(128*1024)][5];
int konto_kasa[(128*1024)];

int konto_buff_index[(128*1024)];
int konto_buff[(128*1024)][16];
int konto_buff_time[(128*1024)][16];
int konto_buff_freq[(128*1024)][16];
int konto_buff_attr1[(128*1024)][16];
int konto_buff_attr2[(128*1024)][16];
byte konto_buff_interrupt[(128*1024)][16];
int konto_quest[(128*1024)][10];
int konto_quest_finnish[(128*1024)][10][8];


//do randoma potrzebne
unsigned int sockets[D_SOCKETS];
int bytes_sent[65536];   //ilosc bajtow wyslanych
int player_revive[65536];
int player_ruch[65536];
int teleport_x = 0;
int teleport_y = 0;
int cycle = 0;  //cykl
int cycleX = 2;  //mnoznik cyklu 1 jesli 4 cykle na sek, 2 jesli 2
int znaleziono_buffa=0;

const int respawn_1 = (60*1*(4/cycleX));  //sekundy
const int respawn_2 = (60*4*(4/cycleX));  //sekundy
const int respawn_3 = (60*10*(4/cycleX));  //sekundy

int w_spirit = 0;
int w_intelect = 0;
int w_stamina = 0;
int w_strength = 0;
int w_agility = 0;

//
int ilosc_kont = 0;


clock_t endwait;
FILE *plik01;
FILE *plik02;
struct timeval tv;
struct sockaddr_in addr;
struct hostent *host;
unsigned int descriptor;
int result;
int result2;
int result_s;
int index;
int delay = 0;
int tymczasowa = 0;
int tymczasowa2 = 0;
int stan_i = 0; //zmienna pomocnicza przy zmianach stanu
int stan_m = 0; //zmienna pomocnicza przy zmianach stanu
int kontrola_predkosci = 0;


//		int klocki = 12;   - przeniesiona jako stala
int zysk_expa = 0;
int regen[65536];
//int odpowiedz = 0;

byte ruch = 0; //tymczasowa
int player_id = 0;
unsigned int sockets_waiting[D_SOCKETS];
int sockets_index = 0;
int sockets_waiting_index = 0;
unsigned int maximun;

char sciezka[40];


char nick[10];
char nick2[10];

int cycle_buffy[65536];

int client[65536];
int player_hp[65536];
int player_mp[65536];
int player_hp_max[65536];
int player_mp_max[65536];
int player_lvl[65536];
int player_exp[65536];
int player_spirit[65536];
int player_klasa[65536];
int player_info[65536];
int player_czas_cast[65536];   //czas od ostatniego casta w cyklach - (1/4) sek
int player_kasa[65536];
int player_bagi[65536][5];
int player_equip[65536][30];
int player_inventory[65536][75];
int player_inventory_ilosc[65536][75];
int player_buff_index[65536];
int player_buff[65536][17];
int player_buff_attr1[65536][16];
int player_buff_attr2[65536][16];
int player_buff_time[65536][16];
int player_buff_freq[65536][16];
byte player_buff_interrupt[65536][16];


int player_x[65536];
int player_y[65536];
int player_map[65536];
byte player_stan[65536][D_SOCKETS];

int player_ekran_x[65536];
int player_ekran_y[65536];


int player_quest[65536][10];
int player_quest_finnish[65536][10][8];


int odpowiedz[65536];   //-80 to wywala (ping timeout)
int nr_konta[65536];   // numer konta
int spelle_l[65536];
int spelle[65536][3];
int spelle_mob[65536][3];
clock_t spelle_endwait[65536];


int nr_post=0;
int i,j,k,l,m,n,o,p,q,r,s,t; //zm pomocnicze

char npc_nick[MAX_MAPS][MAX_CHARS][11];
int npc_gfx[MAX_MAPS][MAX_CHARS];
int npc_type[MAX_MAPS][MAX_CHARS];
int npc_posn[MAX_MAPS][MAX_CHARS]; //0-ilosc ruchow ktora ma dana postac
int npc_pos[MAX_MAPS][MAX_CHARS][MAX_MOVE][2]; //po 2 bajty [x,y] danego ruchu
int npc_pos_x[MAX_MAPS][MAX_CHARS]; //x danego obiektu
int npc_pos_y[MAX_MAPS][MAX_CHARS]; //y danego obiektu
int npc_pos_d[MAX_MAPS][MAX_CHARS]; //droga nastepnego ruchu 0-15
int npc_speed[MAX_MAPS][MAX_CHARS][MAX_MOVE]; //predkosc danego ruchu albo czas postoju
int npc_droga[MAX_MAPS][MAX_CHARS]; //przystanek na ktorym dana postac sie znajduje
int npc_max[MAX_MAPS]; //ilosc postaci na danej mapie
byte npc_stan[MAX_MAPS][MAX_CHARS][D_SOCKETS]; //0 - niewyslany stan 1 - wsylany stan

char mob_nick[MAX_MAPS][MAX_CHARS][11];
int mob_gfx[MAX_MAPS][MAX_CHARS];
int mob_type[MAX_MAPS][MAX_CHARS];
int mob_posn[MAX_MAPS][MAX_CHARS]; //0-ilosc ruchow ktora ma dana postac
int mob_pos[MAX_MAPS][MAX_CHARS][MAX_MOVE][2]; //po 2 bajty [x,y] danego ruchu
int mob_pos_x[MAX_MAPS][MAX_CHARS]; //x danego obiektu
int mob_pos_y[MAX_MAPS][MAX_CHARS]; //y danego obiektu
int mob_pos_d[MAX_MAPS][MAX_CHARS]; //droga nastepnego ruchu 0-15
int mob_speed[MAX_MAPS][MAX_CHARS][MAX_MOVE]; //predkosc danego ruchu albo czas postoju
int mob_droga[MAX_MAPS][MAX_CHARS]; //przystanek na ktorym dana postac sie znajduje
int mob_max[MAX_MAPS]; //ilosc postaci na danej mapie
byte mob_stan[MAX_MAPS][MAX_CHARS][D_SOCKETS]; //0 - niewyslany stan 1 - wsylany stan
int mob_index[MAX_MAPS][MAX_CHARS]; //index_moba w mobs.dat
int mob_hp[MAX_MAPS][MAX_CHARS]; //hit points
int mob_mp[MAX_MAPS][MAX_CHARS]; //mana points
int mob_max_hp[MAX_MAPS][MAX_CHARS]; //hit points max
int mob_max_mp[MAX_MAPS][MAX_CHARS]; //mana points max
int mob_lvl[MAX_MAPS][MAX_CHARS]; //mob's level
int mob_dead[MAX_MAPS][MAX_CHARS]; //mob's death
int mob_target[MAX_MAPS][MAX_CHARS]; //-1 - brak celu
int mob_cz_atak[MAX_MAPS][MAX_CHARS]; //czestotliwosc ataku w cyklach
int mob_moc_atak1[MAX_MAPS][MAX_CHARS]; //atak podstawowy
int mob_moc_atak2[MAX_MAPS][MAX_CHARS]; //atak bonusowy
int mob_atak[MAX_MAPS][MAX_CHARS]; //0 - brak cooldowna na atak (wiecej w cyklach to cooldown)
int mob_loot_il[MAX_MAPS][MAX_CHARS];
int mob_loot_index[MAX_MAPS][MAX_CHARS][64];
int mob_loot_szansa[MAX_MAPS][MAX_CHARS][64];

char buffer[D_INFO];
char buffer_shadow[D_INFO];
char D_HOST[40];

char buffer2[D_INFO];
char buffer3[D_INFO];
