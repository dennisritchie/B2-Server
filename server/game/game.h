#ifndef __GAME__
#define __GAME__
typedef enum __KIND_GAME__{
    INVALID,
    SINGLE,
    PAIR,
    THREE_CARDS,
    FOUR_CARDS,
    SANH_03,
    SANH_04,
    SANH_05,
    SANH_06,
    SANH_07,
    SANH_08,
    SANH_09,
    SANH_10,
    SANH_11,
    SANH_12,
    DOITHONG_03,
    DOITHONG_04,

    FOUR_CARDS_3,
    FOUR_CARDS_H = 50,
    SAU_DOI,
    DOITHONG_05,
    SANH_RONG,
    DOITHONG_06
} KIND_GAME;

const unsigned char CARD_INDEX[] = {'3','4','5','6','7','8','9','I','J','Q','K','A','H'};
const unsigned char CARD_TYPE[] = {0, 1, 2, 3};//BICH, CHUON, RO, CO
extern unsigned char get_card_index(const unsigned char card);
extern unsigned char get_card_type(const unsigned char card);

void set_card_index(unsigned char *card, unsigned char i);
void set_card_type(unsigned char *card, unsigned char t);
void set_card_i_t(unsigned char *card, unsigned char index, unsigned char type);

extern KIND_GAME get_kind(const unsigned char* cards);
//KIND_GAME an_trang(const unsigned char *cards);
//KIND_GAME an_trang(int *cards, char isFirst);
#endif
