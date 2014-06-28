#include "game.h"
#include <string.h>
#include <stdlib.h>

static char check_sanh(const unsigned char *cards);
static KIND_GAME four_cards(const unsigned char *cards);
static char three_cards(const unsigned char *cards);
static char doi_thong(const unsigned char *cards);
static char doi_thong_5(const unsigned char *cards);
static char check_4H(const unsigned char *cards);
static char check_43(const unsigned char *cards);
static char has_pair(const unsigned char *cards, const unsigned char index);

KIND_GAME an_trang(const unsigned char *cards);
KIND_GAME an_trang_ext(const unsigned char *cards,char isFirst);

static void quick_sort(unsigned char *a,  int l, int r);
unsigned char get_card_index(const unsigned char card)
{
    //card & 0011 1100;
    return ( card & 0x3C);
}
unsigned char get_card_type(const unsigned char card)
{
    //card & 0000 0011;
    return ( card & 0x03);
}
void set_card_index(unsigned char *card, unsigned char i)
{
//    memset(card, 0, sizeof(unsigned char));
    (*card) &= 0xC3; //1100 0011
    (*card) |= (i<<2);
}
void set_card_type(unsigned char *card, unsigned char t)
{
//    memset(card, 0, sizeof(unsigned char));
    (*card) &= 0xFC; //1111 1100
    (*card) |= t;
}

void set_card_i_t(unsigned char *card, unsigned char index, unsigned char type)
{
//    0 <= index < 16,
//    0 <= type < 4
    (*card) &= 0xC3; //1100 0011
    (*card) |= (index<<2);

    (*card) &= 0xFC; //1111 1100
    (*card) |= type;
}

KIND_GAME get_kind(const char unsigned * cards)
{
    KIND_GAME rs = INVALID;
    char n = strlen(cards);
    if(n == 1){
        rs = SINGLE;
    }
    else if ( n == 2){
        unsigned char card_0 = get_card_index(cards[0]);
        unsigned char card_1 = get_card_index(cards[1]);
        if(card_0 == card_1){
            rs = PAIR;
            return rs;
        }
    }
    else if ( n == 3){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_03;
        }
        /*****/
        set= three_cards(cards);
        if(set){
            return THREE_CARDS;
        }
    }
    else if ( n == 4){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_04;
        }
        /*****/
        return four_cards(cards);
    }
    else if ( n == 5){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_05;
        }
    }
    else if ( n == 6){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_06;
        }
    }
    else if ( n == 7){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_07;
        }
    }
    else if ( n == 8){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_08;
        }
    }
    else if ( n == 9){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_09;
        }
    }
    else if ( n == 10){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_10;
        }
    }
    else if ( n == 11){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_11;
        }
    }
    else if ( n == 12){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_12;
        }
    }
    else if ( n == 13){
        char set = 0;
        set = check_sanh(cards);
        if(set){
            return SANH_RONG;
        }
    }

    return rs;
}

char check_sanh(const unsigned char *cards){
    char rs = 1;
    int n = strlen(cards);
    char i = 0;
    while(i<(n-1)){
        unsigned char i0 = get_card_index(cards[i]);
        unsigned char i1 = get_card_index(cards[i+1]);
        if( i1 != (i0+1)){
            rs = 0;
            break;
        }
        ++i;
    }
    return rs;
}
KIND_GAME four_cards(const unsigned char *cards)
{
    KIND_GAME rs = INVALID;
    char n = strlen(cards);
    char i = 0;
    if(n != 4){
        return rs;
    }
    char set = 1;
    while(i < (n-1)){
        unsigned char i0 = get_card_index(cards[i]);
        unsigned char i1 = get_card_index(cards[i+1]);
        if( i1 != i0){
            set = 0;
            break;
        }
        ++i;
    }
    if(set){
        rs = FOUR_CARDS;
        if(get_card_index(cards[i]) == strlen(CARD_INDEX) -1){
            rs = FOUR_CARDS_H;
        }
    }
    return rs;
}
char three_cards(const unsigned char *cards){
    char rs = 1;
    char i = 0;
    int n = strlen(cards);
    while(i < (n-1)){
        unsigned char i0 = get_card_index(cards[i]);
        unsigned char i1 = get_card_index(cards[i+1]);
        if( i1 != i0){
            rs = 0;
            break;
        }
        ++i;
    }
    return rs;
}
static char doi_thong(const unsigned char *cards){
    char rs = 1;
    int n = strlen(cards);
    char i = 0;
    if(n%2 != 0){
        return 0;
    }
    unsigned char in = get_card_index(cards[n-1]);
    if(in == (strlen(CARD_INDEX) - 1)){
        return 0;
    }
    while(i < (n - 2)){
        unsigned char i0 = get_card_index(cards[i]);
        unsigned char i1 = get_card_index(cards[i+1]);
        if( i1 != i0){
            rs = 0;
            break;
        }
        if(i+2 > (n-1)){
            break;
        }
        unsigned char i2 = get_card_index(cards[i+2]);
        if( (i0+1) != i2){
            rs = 0;
            break;
        }
        i += 2;
    }
    return rs;
}
KIND_GAME an_trang(const unsigned char *cards){
    KIND_GAME rs = INVALID;
    if(strlen(cards) != strlen(CARD_INDEX)){
        return INVALID;
    }
    char set = check_sanh(cards);
    if(set){
        return SANH_RONG;
    }
    set = doi_thong_5(cards);
    if(set){
        if(set == 5) {return DOITHONG_05;}
        if(set == 6) {return DOITHONG_06;}
    }
    set = check_4H(cards);
    if(set){ return FOUR_CARDS_H;}
    set = check_43(cards);
    if(set){ return FOUR_CARDS_3;}
    return rs;
}
KIND_GAME an_trang_ext(const unsigned char *cards,char isFirst){
    KIND_GAME rs = an_trang(cards);
    if(rs != INVALID) { return rs;}
    if(!isFirst) {return rs;}
    return rs;
}
static inline int partition( unsigned char *a, int l, int r) {
   unsigned char pivot, i, j, t;
   pivot = a[l];
   i = l; j = r+1;

   while( 1)
   {
        do ++i; while( a[i] <= pivot && i <= r );
        do --j; while( a[j] > pivot );
        if( i >= j ) break;
        t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}

void quick_sort(unsigned char *a,int l, int r){
    unsigned char j;

    if( l < r )
    {
         // divide and conquer
        j = partition( a, l, r);
        quick_sort( a, l, j-1);
        quick_sort( a, j+1, r);
    }
}
char doi_thong_5(const unsigned char *cards){
    char rs = 0;
    char n = 6, b = 6;
    return rs;
}
char check_4H(const unsigned char *cards){
    char rs = 0;
    return rs;
}
char check_43(const unsigned char *cards){
    char rs = 0;
    return rs;
}
char has_pair(const unsigned char * cards, const unsigned char index)
{
//    char rs = 0;
    unsigned char a = get_card_index(cards[index]);
    if(index > 0){
        unsigned char b = get_card_index(cards[index - 1]);
        if(a == b){
            return 1;
        }
    }
    if(index < 12){
        unsigned char n = get_card_index(cards[index + 1]);
        if(a == n){
            return 1;
        }
    }
    return 0;
}
