#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#pragma warning (disable:4996)
typedef struct {
    char username[1000];
    char password[1000];
    char email[1000];
}file_id;
file_id fi;
typedef struct {
    char username[1000];
    char password[1000];
    char email[1000];
}scanf_id;
scanf_id si;
typedef struct {
    char user1[1000];
    char user2[1000];
}user_info;
user_info ui;
typedef struct {
    char player1[1000];
    char player2[1000];
    int game_status;
}match_result;
//game_status= 0 for mosavi, 1 plaer1 won, 2 player2 won
match_result mr;
int result;
int livesuser1 = 0;
int livesuser2 = 0;
enum lastmove {
    stable,
    up,
    down,
    right,
    left
};
enum lastmove bullet1 = stable;
enum lastmove bullet1_a = stable;
enum lastmove bullet2 = stable;
enum lastmove bullet2_a = stable;
int round_won1 = 0;
int round_won2 = 0;
COORD bullet_1 = { -1,-1 };
COORD bullet_2 = { -1,-1 };
COORD user1r1_fix = { 9,6 };
COORD user2r1_fix = { 52,6 };
COORD user1r1 = { 9,6 };
COORD user2r1 = { 52,6 };
COORD heart = { 29,3 };
COORD heartr2 = { 35,9 };
COORD bomb = { 35,13 };
COORD gun_up1 = { 51,7 };
COORD gun_up2 = { 20,7 };
COORD ghost = { 35,2 };
COORD user1r2_fix = { 7,10 };
COORD user2r2_fix = { 62,10 };
COORD user1r2 = { 7,10 };
COORD user2r2 = { 62,10 };
int gunuser1 = 0;
int gunuser2 = 0;
int ghostuser1 = 0;
int ghostuser2 = 0;
int bombuser1 = 0;
int bombuser2 = 0;
char round1_matrix[15][60];
int walls1_matrix[15][60];
int mirror_matrix[15][60];
char round2_matrix[20][70];
int walls2_matrix[20][70];
int mirror2_matrix[20][70];
void clear() {
    system("@cls||clear");
}
void scanf_string(char* string) {
    do {
        scanf("%s", string);
    } while (string[0] == '\n' || string[0] == ' ');
}
int valid_user(char* username) {
    for (int i = 0; i < strlen(username); i++) {
        char c = username[i];
        if (!((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || c == '_' || c == '-' || (c >= '0' && c <= '9') || c == '.')) {
            return 0;
        }
    }
    return 1;
}
int valid_email(char* email) {
    int r = 0;
    int atcine = 0;
    int c = 0;
    int o = 0;
    int m = 0;
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') {
            atcine = 1;
            r = i;
        }
        if (i > r && atcine == 1) {
            if (email[i] == 'c') c = 1;
            if (email[i] == 'o') o = 1;
            if (email[i] == 'm') m = 1;
        }
    }
    if (atcine && c && o && m)return 1;
    else return 0;
}
int get_digit() {
    int result = -1;
    while (result == -1)
    {
        char inp = getchar();
        if (inp >= '0' && inp <= '9')
            result = inp - '0';
    }

    return result;
}
int find_user(char* username, file_id fi, FILE* user_info) {

    while (fread(&fi, sizeof(file_id), 1, user_info)) {
        if (strcmp(fi.username, username) == 0) {
            return 1;
        }
    }
    return 0;
}
void Sign_up() {
    FILE* user_info = NULL;
    user_info = fopen("data.txt", "r");
    if (user_info == NULL)
    {
        exit(1);
    }
    while (1) {
        clear();
        printf("Type your Username :");
        scanf_string(si.username);
        fseek(user_info, 0, SEEK_SET);
        if (!valid_user(si.username)) {
            printf("\nYour Username is invalid!");
            Sleep(2000);
            continue;
        }
        else if (find_user(si.username, fi, user_info)) {
            printf("\nYour username has already been used\n");
            Sleep(2000);
            continue;
        }
        else break;
    }
    fclose(user_info);
    clear();
    char repeat[1000];
    while (1) {
        printf("Password(at least 8 character):");
        scanf_string(si.password);
        if (strlen(si.password) < 8) {
            printf("\nPassword is less than 8 characters");
            Sleep(1000);
            clear();
            continue;
        }
        else break;
    }
    while (1) {
        printf("\nType Password again:");
        scanf_string(repeat);
        if (strcmp(si.password, repeat) != 0) {
            printf("Password repetition does not match");
            Sleep(1000);
            clear();
            continue;
        }
        else break;
    }
    Sleep(1000);
    clear();
    while (1) {
        clear();
        printf("\nType your Email:");
        scanf_string(si.email);
        if (!valid_email(si.email)) {
            clear();
            printf("\nYour Email is invalid!");
            Sleep(2000);
            continue;
        }
        else break;
    }
    Sleep(1000);
    clear();
    user_info = fopen("data.txt", "a");
    fwrite(&si, sizeof(scanf_id), 1, user_info);
    fclose(user_info);
}
void star_password(char* password) {
    char ch;
    int i = 0;
    while ((ch = getch()) != '\r') {
        if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        }
        else {
            if (i < 100) {
                password[i++] = ch;
                printf("*");
            }
        }
    }
    password[i] = '\0';
}
void Sign_in() {
    while (1) {
        clear();
        printf("1)Log in\n2)Forgot Password\nType correct number:");
        int condition = get_digit();
        if (condition == 1) {
            FILE* user_info = fopen("data.txt", "r");
            clear();
            while (1) {
                printf("Username:");
                scanf_string(si.username);
                fseek(user_info, 0, SEEK_SET);
                if (!find_user(si.username, fi, user_info)) {
                    printf("\nUsername doesn't exist");
                    Sleep(2000);
                    clear();
                    continue;
                }
                else break;
            }
            while (1) {
                clear();
                fseek(user_info, 0, SEEK_SET);
                while (fread(&fi, sizeof(file_id), 1, user_info)) {
                    if (strcmp(fi.username, si.username) == 0)break;
                }
                printf("Username: %s", si.username);
                printf("\nPassword:");
                star_password(si.password);
                if (strcmp(fi.password, si.password) != 0) {
                    printf("\nyour password is incorrect");
                    Sleep(2000);
                    continue;
                }
                else break;
            }
            fclose(user_info);
            break;
        }
        else if (condition == 2) {
            clear();
            FILE* user_info = fopen("data.txt", "r+");
            fseek(user_info, 0, SEEK_SET);
            while (1) {
                printf("Enter your Username to change Password:");
                scanf_string(si.username);
                fseek(user_info, 0, SEEK_SET);
                if (!find_user(si.username, fi, user_info)) {
                    printf("\nUsername doesn't exist");
                    Sleep(2000);
                    clear();
                    continue;
                }
                else break;
            }
            while (1) {
                clear();
                fseek(user_info, 0, SEEK_SET);
                while (fread(&fi, sizeof(file_id), 1, user_info)) {
                    if (strcmp(fi.username, si.username) == 0)break;
                }
                printf("Username: %s", si.username);
                printf("\nEnter your Email:");
                scanf_string(si.email);
                if (strcmp(fi.email, si.email) != 0) {
                    printf("your Email doesn't exist");
                    Sleep(1000);
                    continue;
                }
                else break;
            }
            char repeat[1000];
            while (1) {
                printf("Enter New Password(at least 8 character):");
                scanf_string(si.password);
                if (strlen(si.password) < 8) {
                    printf("\nUsername is less than 8 characters");
                    Sleep(1000);
                    clear();
                    continue;
                }
                else break;
            }
            while (1) {
                printf("\nType Password again:");
                scanf_string(repeat);
                if (strcmp(si.password, repeat) != 0) {
                    printf("Password repetition does not match");
                    Sleep(1000);
                    clear();
                    continue;
                }
                else break;
            }
            strcpy(fi.password, si.password);
            fseek(user_info, -(int)sizeof(file_id), SEEK_CUR);
            fwrite(&fi, sizeof(file_id), 1, user_info);
            fclose(user_info);
            clear();
        }
    }
}
void login_page() {
    while (1) {
        clear();
        printf("LOGIN PAGE\n1) Sign up\n2)Sign in\n3)Exit\nType right number :");
        int input = get_digit();
        if (input == 1) {
            clear();
            Sign_up();

        }
        else if (input == 2) {
            clear();
            Sign_in();
            break;
        }
        else if (input == 3) exit(0);

    }
}
void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE), c);
}
COORD moveDirection(COORD position, int dx, int dy) {
    position.X += dx;
    position.Y += dy;
    return position;
}
BOOL keyPressed(int key) {
    return GetAsyncKeyState(key);
}
void map1() {
    int wall_coords[40][2] = {
           {4, 29},
           {5, 29},
           {6, 29},
           {8, 29},
           {9, 29},
           {10, 29},
           {4,8},
           {4,9},
           {4,10},
           {5,10},
           {6,10},
           {7,10},
           {8,10},
           {9,10},
           {9,9},
           {9,8},
           {4,53},
           {4,52},
           {4,51},
           {5,51},
           {6,51},
           {7,51},
           {8,51},
           {9,51},
           {9,52},
           {9,53},
           {5,33},
           {5,34},
           {5,35},
           {9,25},
           {9,24},
           {9,23},
           {11,43},
           {12,43},
           {2,43},
           {3,43},
           {11,16},
           {12,16},
           {2,16},
           {3,16},
    };

    for (int i = 0; i < 15; i++) {
        walls1_matrix[i][0] = 1;
        walls1_matrix[i][59] = 1;
    }
    for (int j = 0; j < 60; j++) {
        walls1_matrix[0][j] = 1;
        walls1_matrix[14][j] = 1;
    }

    for (int l = 0; l < 40; l++) {
        int i = wall_coords[l][0], j = wall_coords[l][1];
        walls1_matrix[i][j] = 1;
    }
    mirror_matrix[7][29] = 1;
    mirror_matrix[9][11] = 1;
    mirror_matrix[8][11] = 1;
    mirror_matrix[5][11] = 1;
    mirror_matrix[4][11] = 1;
    mirror_matrix[9][50] = 1;
    mirror_matrix[8][50] = 1;
    mirror_matrix[5][50] = 1;
    mirror_matrix[4][50] = 1;
    mirror_matrix[2][17] = 1;
    mirror_matrix[3][17] = 1;
    mirror_matrix[11][42] = 1;
    mirror_matrix[12][42] = 1;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 60; j++) {
            if (walls1_matrix[i][j])
                round1_matrix[i][j] = 219;
            else if (mirror_matrix[i][j])
                round1_matrix[i][j] = 186;
            else
                round1_matrix[i][j] = ' ';
        }
    }
    round1_matrix[3][46] = '@';//siahchae_r
    round1_matrix[9][17] = '@';//sihchale_l
}
void movebullet1() {
    if (keyPressed('C')) {
        bullet_1 = user1r1;
        if (bullet1 != stable)
            bullet1 = bullet1_a;
        else bullet1 = bullet1_a;
    }
    if (bullet1 == up) {
        if (round1_matrix[bullet_1.Y - 1][bullet_1.X] == (char)219) {
            bullet1 = stable;
            bullet_1.X = -1;
            bullet_1.Y = -1;

        }
        else if (round1_matrix[bullet_1.Y - 1][bullet_1.X] == (char)186) {
            bullet1 = down;
        }
        else if (round1_matrix[bullet_1.Y - 1][bullet_1.X] == ' ' || round1_matrix[bullet_1.Y - 1][bullet_1.X] == '@') {
            bullet_1.Y -= 1;
            if (bullet_1.Y == user1r1.Y && bullet_1.X == user1r1.X) {
                user1r1 = user1r1_fix;
                livesuser1 -= 1;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
            if (bullet_1.Y == user2r1.Y && bullet_1.X == user2r1.X) {
                user2r1 = user2r1_fix;
                livesuser2 -= 1;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
        }
    }
    if (bullet1 == down) {
        if (round1_matrix[bullet_1.Y + 1][bullet_1.X] == (char)219) {
            bullet1 = stable;
            bullet_1.X = -1;
            bullet_1.Y = -1;

        }
        else if (round1_matrix[bullet_1.Y + 1][bullet_1.X] == (char)186) {
            bullet1 = up;
        }
        else if (round1_matrix[bullet_1.Y + 1][bullet_1.X] == ' ' || round1_matrix[bullet_1.Y + 1][bullet_1.X] == '@') {
            bullet_1.Y += 1;
            if (bullet_1.Y == user1r1.Y && bullet_1.X == user1r1.X) {
                user1r1 = user1r1_fix;
                livesuser1 -= 1;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
            if (bullet_1.Y == user2r1.Y && bullet_1.X == user2r1.X) {
                user2r1 = user2r1_fix;
                livesuser2 -= 1;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
        }
    }
    if (bullet1 == right) {
        if (round1_matrix[bullet_1.Y][bullet_1.X + 1] == (char)219) {
            bullet1 = stable;
            bullet_1.X = -1;
            bullet_1.Y = -1;

        }
        else if (round1_matrix[bullet_1.Y][bullet_1.X + 1] == (char)186) {
            bullet1 = left;
        }
        else if (round1_matrix[bullet_1.Y][bullet_1.X + 1] == ' ' || round1_matrix[bullet_1.Y][bullet_1.X + 1] == '@') {
            bullet_1.X += 1;
            if (bullet_1.Y == user1r1.Y && bullet_1.X == user1r1.X) {
                user1r1 = user1r1_fix;
                livesuser1 -= 1;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
            if (bullet_1.Y == user2r1.Y && bullet_1.X == user2r1.X) {
                user2r1 = user2r1_fix;
                livesuser2 -= 1;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
        }
    }
    if (bullet1 == left) {
        if (round1_matrix[bullet_1.Y][bullet_1.X - 1] == (char)219) {
            bullet1 = stable;
            bullet_1.X = -1;
            bullet_1.Y = -1;

        }
        else if (round1_matrix[bullet_1.Y][bullet_1.X - 1] == (char)186) {
            bullet1 = right;
        }
        else if (round1_matrix[bullet_1.Y][bullet_1.X - 1] == ' ' || round1_matrix[bullet_1.Y][bullet_1.X - 1] == '@') {
            bullet_1.X -= 1;
            if (bullet_1.Y == user1r1.Y && bullet_1.X == user1r1.X) {
                user1r1 = user1r1_fix;
                livesuser1 -= 1;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
            if (bullet_1.Y == user2r1.Y && bullet_1.X == user2r1.X) {
                user2r1 = user2r1_fix;
                livesuser2 -= 1;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
        }
    }
}
void moveuser1() {
    if (keyPressed('W') && (user1r1.Y - 1 > 0)) {
        if (bullet1 == stable) bullet1 = up;
        bullet1_a = up;
        if (round1_matrix[user1r1.Y - 1][user1r1.X] == ' ') {
            user1r1 = moveDirection(user1r1, 0, -1);
            if (user1r1.X == 29 && user1r1.Y == 3 && heart.X != 0) {
                heart.X = 0;
                livesuser1 += 5;
            }
            else if (user1r1.X == 53 && user1r1.Y == 11) {
                user1r1.X = 8;
                user1r1.Y = 11;
            }
            else if (user1r1.X == 8 && user1r1.Y == 11) {
                user1r1.X = 53;
                user1r1.Y = 11;
            }
        }
        if (round1_matrix[user1r1.Y - 1][user1r1.X] == '@') {
            user1r1 = user1r1_fix;
        }
    }
    if (keyPressed('S') && (user1r1.Y + 1 < 14)) {
        if (bullet1 == stable) bullet1 = down;
        bullet1_a = down;
        if (round1_matrix[user1r1.Y + 1][user1r1.X] == ' ') {
            user1r1 = moveDirection(user1r1, 0, +1);
            if (user1r1.X == 29 && user1r1.Y == 3 && heart.X != 0) {
                heart.X = 0;
                livesuser1 += 5;
            }
            else if (user1r1.X == 53 && user1r1.Y == 11) {
                user1r1.X = 8;
                user1r1.Y = 11;
            }
            else if (user1r1.X == 8 && user1r1.Y == 11) {
                user1r1.X = 53;
                user1r1.Y = 11;
            }
        }
        if (round1_matrix[user1r1.Y + 1][user1r1.X] == '@') {
            user1r1 = user1r1_fix;
        }

    }
    if (keyPressed('A') && (user1r1.X - 1 > 0)) {
        if (bullet1 == stable) bullet1 = left;
        bullet1_a = left;
        if (round1_matrix[user1r1.Y][user1r1.X - 1] == ' ') {
            user1r1 = moveDirection(user1r1, -1, 0);
            if (user1r1.X == 29 && user1r1.Y == 3 && heart.X != 0) {
                heart.X = 0;
                livesuser1 += 5;
            }
            else if (user1r1.X == 53 && user1r1.Y == 11) {
                user1r1.X = 8;
                user1r1.Y = 11;
            }
            else if (user1r1.X == 8 && user1r1.Y == 11) {
                user1r1.X = 53;
                user1r1.Y = 11;
            }
        }
        if (round1_matrix[user1r1.Y][user1r1.X - 1] == '@') {
            user1r1 = user1r1_fix;
        }
    }
    if (keyPressed('D') && (user1r1.X + 1 < 59)) {
        if (bullet1 == stable) bullet1 = right;
        bullet1_a = right;
        if (round1_matrix[user1r1.Y][user1r1.X + 1] == ' ') {
            user1r1 = moveDirection(user1r1, 1, 0);
            if (user1r1.X == 29 && user1r1.Y == 3 && heart.X != 0) {
                heart.X = 0;
                livesuser1 += 5;
            }
            else if (user1r1.X == 53 && user1r1.Y == 11) {
                user1r1.X = 8;
                user1r1.Y = 11;
            }
            else if (user1r1.X == 8 && user1r1.Y == 11) {
                user1r1.X = 53;
                user1r1.Y = 11;
            }
        }
        if (round1_matrix[user1r1.Y][user1r1.X + 1] == '@') {
            user1r1 = user1r1_fix;
        }

    }
    movebullet1();
}
void movebullet2() {
    if (keyPressed('N')) {
        bullet_2 = user2r1;
        if (bullet2 != stable)
            bullet2 = bullet2_a;
        else bullet2 = bullet2_a;
    }
    if (bullet2 == up) {
        if (round1_matrix[bullet_2.Y - 1][bullet_2.X] == (char)219) {
            bullet2 = stable;
            bullet_2.X = -1;
            bullet_2.Y = -1;

        }
        else if (round1_matrix[bullet_2.Y - 1][bullet_2.X] == (char)186) {
            bullet2 = down;
        }
        else if (round1_matrix[bullet_2.Y - 1][bullet_2.X] == ' ' || round1_matrix[bullet_2.Y - 1][bullet_2.X] == '@') {
            bullet_2.Y -= 1;
            if (bullet_2.Y == user1r1.Y && bullet_2.X == user1r1.X) {
                user1r1 = user1r1_fix;
                livesuser1 -= 1;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
            if (bullet_2.Y == user2r1.Y && bullet_2.X == user2r1.X) {
                user2r1 = user2r1_fix;
                livesuser2 -= 1;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
        }
    }
    if (bullet2 == down) {
        if (round1_matrix[bullet_2.Y + 1][bullet_2.X] == (char)219) {
            bullet2 = stable;
            bullet_2.X = -1;
            bullet_2.Y = -1;

        }
        else if (round1_matrix[bullet_2.Y + 1][bullet_2.X] == (char)186) {
            bullet2 = up;
        }
        else if (round1_matrix[bullet_2.Y + 1][bullet_2.X] == ' ' || round1_matrix[bullet_2.Y + 1][bullet_2.X] == '@') {
            bullet_2.Y += 1;
            if (bullet_2.Y == user1r1.Y && bullet_2.X == user1r1.X) {
                user1r1 = user1r1_fix;
                livesuser1 -= 1;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
            if (bullet_2.Y == user2r1.Y && bullet_2.X == user2r1.X) {
                user2r1 = user2r1_fix;
                livesuser2 -= 1;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
        }
    }
    if (bullet2 == right) {
        if (round1_matrix[bullet_2.Y][bullet_2.X + 1] == (char)219) {
            bullet2 = stable;
            bullet_2.X = -1;
            bullet_2.Y = -1;

        }
        else if (round1_matrix[bullet_2.Y][bullet_2.X + 1] == (char)186) {
            bullet2 = left;
        }
        else if (round1_matrix[bullet_2.Y][bullet_2.X + 1] == ' ' || round1_matrix[bullet_2.Y][bullet_2.X + 1] == '@') {
            bullet_2.X += 1;
            if (bullet_2.Y == user1r1.Y && bullet_2.X == user1r1.X) {
                user1r1 = user1r1_fix;
                livesuser1 -= 1;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
            if (bullet_2.Y == user2r1.Y && bullet_2.X == user2r1.X) {
                user2r1 = user2r1_fix;
                livesuser2 -= 1;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
        }
    }
    if (bullet2 == left) {
        if (round1_matrix[bullet_2.Y][bullet_2.X - 1] == (char)219) {
            bullet2 = stable;
            bullet_2.X = -1;
            bullet_2.Y = -1;

        }
        else if (round1_matrix[bullet_2.Y][bullet_2.X - 1] == (char)186) {
            bullet2 = right;
        }
        else if (round1_matrix[bullet_2.Y][bullet_2.X - 1] == ' ' || round1_matrix[bullet_2.Y][bullet_2.X - 1] == '@') {
            bullet_2.X -= 1;
            if (bullet_2.Y == user1r1.Y && bullet_2.X == user1r1.X) {
                user1r1 = user1r1_fix;
                livesuser1 -= 1;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
            if (bullet_2.Y == user2r1.Y && bullet_2.X == user2r1.X) {
                user2r1 = user2r1_fix;
                livesuser2 -= 1;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
        }
    }
}
void moveuser2() {
    if (keyPressed('I') && (user2r1.Y - 1 > 0)) {
        if (bullet2 == stable) bullet2 = up;
        bullet2_a = up;
        if (round1_matrix[user2r1.Y - 1][user2r1.X] == ' ') {
            user2r1 = moveDirection(user2r1, 0, -1);
            if (user2r1.X == 29 && user2r1.Y == 3 && heart.X != 0) {
                heart.X = 0;
                livesuser2 += 5;
            }
            else if (user2r1.X == 53 && user2r1.Y == 11) {
                user2r1.X = 8;
                user2r1.Y = 11;
            }
            else if (user2r1.X == 8 && user2r1.Y == 11) {
                user2r1.X = 53;
                user2r1.Y = 11;
            }
        }
        if (round1_matrix[user2r1.Y - 1][user2r1.X] == '@') {
            user2r1 = user2r1_fix;
        }
    }
    if (keyPressed('K') && (user2r1.Y + 1 < 14)) {
        if (bullet2 == stable) bullet2 = down;
        bullet2_a = down;
        if (round1_matrix[user2r1.Y + 1][user2r1.X] == ' ') {
            user2r1 = moveDirection(user2r1, 0, +1);
            if (user2r1.X == 29 && user2r1.Y == 3 && heart.X != 0) {
                heart.X = 0;
                livesuser2 += 5;
            }
            else if (user2r1.X == 53 && user2r1.Y == 11) {
                user2r1.X = 8;
                user2r1.Y = 11;
            }
            else if (user2r1.X == 8 && user2r1.Y == 11) {
                user2r1.X = 53;
                user2r1.Y = 11;
            }
        }
        if (round1_matrix[user2r1.Y + 1][user2r1.X] == '@') {
            user2r1 = user2r1_fix;
        }

    }
    if (keyPressed('J') && (user2r1.X - 1 > 0)) {
        if (bullet2 == stable) bullet2 = left;
        bullet2_a = left;
        if (round1_matrix[user2r1.Y][user2r1.X - 1] == ' ') {
            user2r1 = moveDirection(user2r1, -1, 0);
            if (user2r1.X == 29 && user2r1.Y == 3 && heart.X != 0) {
                heart.X = 0;
                livesuser2 += 5;
            }
            else if (user2r1.X == 53 && user2r1.Y == 11) {
                user2r1.X = 8;
                user2r1.Y = 11;
            }
            else if (user2r1.X == 8 && user2r1.Y == 11) {
                user2r1.X = 53;
                user2r1.Y = 11;
            }
        }
        if (round1_matrix[user2r1.Y][user2r1.X - 1] == '@') {
            user2r1 = user2r1_fix;
        }
    }
    if (keyPressed('L') && (user2r1.X + 1 < 59)) {
        if (bullet2 == stable) bullet2 = right;
        bullet2_a = right;
        if (round1_matrix[user2r1.Y][user2r1.X + 1] == ' ') {
            user2r1 = moveDirection(user2r1, 1, 0);
            if (user2r1.X == 29 && user2r1.Y == 3 && heart.X != 0) {
                heart.X = 0;
                livesuser2 += 5;
            }
            else if (user2r1.X == 53 && user2r1.Y == 11) {
                user2r1.X = 8;
                user2r1.Y = 11;
            }
            else if (user2r1.X == 8 && user2r1.Y == 11) {
                user2r1.X = 53;
                user2r1.Y = 11;
            }
        }
        if (round1_matrix[user2r1.Y][user2r1.X + 1] == '@') {
            user2r1 = user2r1_fix;
        }

    }
    movebullet2();
}
void map2() {
    int wall_coords[54][2] = {
        {2,20},
        {3,20},
        {15,20},
        {16,20},
        {2,49},
        {3,49},
        {15,49},
        {16,49},
        {4,33},
        {4,34},
        {4,35},
        {4,36},
        {4,37},
        {14,33},
        {14,34},
        {14,35},
        {14,36},
        {14,37},
        {6,35},
        {7,35},
        {7,34},
        {7,33},
        {7,32},
        {7,31},
        {8,31},
        {9,31},
        {9,30},
        {9,29},
        {8,43},
        {8,42},
        {8,41},
        {8,40},
        {9,40},
        {10,40},
        {10,39},
        {10,38},
        {10,37},
        {10,36},
        {10,35},
        {11,35},
        {9,49},
        {8,48},
        {7,47},
        {6,46},
        {10,48},
        {11,47},
        {12,46},
        {9,23},
        {8,24},
        {7,25},
        {6,26},
        {10,24},
        {11,25},
        {12,26},
    };
    for (int l = 0; l < 54; l++) {
        int i = wall_coords[l][0], j = wall_coords[l][1];
        walls2_matrix[i][j] = 1;
    }
    for (int x = 0; x < 70; x++) {
        walls2_matrix[0][x] = 1;
        walls2_matrix[19][x] = 1;
    }
    for (int y = 0; y < 20; y++) {
        walls2_matrix[y][0] = 1;
        walls2_matrix[y][69] = 1;
    }
    for (int i = 6; i < 15; i++)
        mirror2_matrix[i][58] = 1;
    for (int i = 6; i < 15; i++)
        mirror2_matrix[i][11] = 1;
    mirror2_matrix[9][32] = 1;
    mirror2_matrix[8][47] = 1;
    mirror2_matrix[10][25] = 1;
    mirror2_matrix[2][21] = 1;
    mirror2_matrix[3][48] = 1;
    mirror2_matrix[15][21] = 1;
    mirror2_matrix[16][48] = 1;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 70; j++) {
            if (walls2_matrix[i][j])
                round2_matrix[i][j] = 219;
            else if (mirror2_matrix[i][j])
                round2_matrix[i][j] = 186;
            else
                round2_matrix[i][j] = ' ';
        }
    }
    round2_matrix[8][37] = '@';
    round2_matrix[10][32] = '@';
    round2_matrix[12][35] = '@';
}
void movebullet1r2() {
    if (keyPressed('C')) {
        bullet_1 = user1r2;
        if (bullet1 != stable)
            bullet1 = bullet1_a;
        else bullet1 = bullet1_a;
        if (ghostuser1 >= 0) --ghostuser1;
        if (gunuser1 >= 0) --gunuser1;
        if (bombuser1 >= 0) --bombuser1;
    }
    if (bullet1 == up) {
        if (round2_matrix[bullet_1.Y - 1][bullet_1.X] == (char)219) {
            if (ghostuser1 >= 0) {
                if (bullet_1.Y - 1 == 0) {
                    bullet1 = stable;
                    bullet_1.X = -1;
                    bullet_1.Y = -1;
                }
                else bullet_1.Y -= 1;
            }
            else if (bombuser1 >= 0) {
                if ((user1r2.Y <= bullet_1.Y + 1 && user1r2.Y >= bullet_1.Y - 3) && (user1r2.X <= bullet_1.X + 2 && user1r2.X >= bullet_1.X - 2)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_1.Y + 1 && user2r2.Y >= bullet_1.Y - 3) && (user2r2.X <= bullet_1.X + 2 && user2r2.X >= bullet_1.X - 2)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
            else {
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
        }
        else if (round2_matrix[bullet_1.Y - 1][bullet_1.X] == (char)186) {
            if (bombuser1 >= 0) {
                round2_matrix[bullet_1.Y - 1][bullet_1.X] = ' ';
                if ((user1r2.Y <= bullet_1.Y && user1r2.Y >= bullet_1.Y - 2) && (user1r2.X <= bullet_1.X + 1 && user1r2.X >= bullet_1.X - 1)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_1.Y && user2r2.Y >= bullet_1.Y - 2) && (user2r2.X <= bullet_1.X + 1 && user2r2.X >= bullet_1.X - 1)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
            else bullet1 = down;
        }
        else if (round2_matrix[bullet_1.Y - 1][bullet_1.X] == ' ' || round2_matrix[bullet_1.Y - 1][bullet_1.X] == '@') {
            bullet_1.Y -= 1;
            if (bullet_1.Y == user1r2.Y && bullet_1.X == user1r2.X) {
                if (gunuser1 >= 0)livesuser1 -= 2;
                else livesuser1 -= 1;
                user1r2 = user1r2_fix;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
            if (bullet_1.Y == user2r2.Y && bullet_1.X == user2r2.X) {
                if (gunuser1 >= 0)livesuser2 -= 2;
                else livesuser2 -= 1;
                user2r2 = user2r2_fix;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
        }
    }
    if (bullet1 == down) {
        if (round2_matrix[bullet_1.Y + 1][bullet_1.X] == (char)219) {
            if (ghostuser1 >= 0) {
                if (bullet_1.Y + 1 == 19) {
                    bullet1 = stable;
                    bullet_1.X = -1;
                    bullet_1.Y = -1;
                }
                else bullet_1.Y += 1;
            }
            else if (bombuser1 >= 0) {
                if ((user1r2.Y <= bullet_1.Y + 3 && user1r2.Y >= bullet_1.Y - 1) && (user1r2.X <= bullet_1.X + 2 && user1r2.X >= bullet_1.X - 2)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_1.Y + 3 && user2r2.Y >= bullet_1.Y - 1) && (user2r2.X <= bullet_1.X + 2 && user2r2.X >= bullet_1.X - 2)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
            else {
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }

        }
        else if (round2_matrix[bullet_1.Y + 1][bullet_1.X] == (char)186) {
            if (bombuser1 >= 0) {
                round2_matrix[bullet_1.Y + 1][bullet_1.X] = ' ';
                if ((user1r2.Y <= bullet_1.Y + 2 && user1r2.Y >= bullet_1.Y) && (user1r2.X <= bullet_1.X + 1 && user1r2.X >= bullet_1.X - 1)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_1.Y + 2 && user2r2.Y >= bullet_1.Y) && (user2r2.X <= bullet_1.X + 1 && user2r2.X >= bullet_1.X - 1)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
            else bullet1 = up;
        }
        else if (round2_matrix[bullet_1.Y + 1][bullet_1.X] == ' ' || round2_matrix[bullet_1.Y + 1][bullet_1.X] == '@') {
            bullet_1.Y += 1;
            if (bullet_1.Y == user1r2.Y && bullet_1.X == user1r2.X) {
                if (gunuser1 >= 0)livesuser1 -= 2;
                else livesuser1 -= 1;
                user1r2 = user1r2_fix;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
            if (bullet_1.Y == user2r2.Y && bullet_1.X == user2r2.X) {
                if (gunuser1 >= 0)livesuser2 -= 2;
                else livesuser2 -= 1;
                user2r2 = user2r2_fix;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
        }
    }
    if (bullet1 == right) {
        if (round2_matrix[bullet_1.Y][bullet_1.X + 1] == (char)219) {
            if (ghostuser1 >= 0) {
                if (bullet_1.X + 1 == 69) {
                    bullet1 = stable;
                    bullet_1.X = -1;
                    bullet_1.Y = -1;
                }
                else bullet_1.X += 1;
            }
            else if (bombuser1 >= 0) {
                if ((user1r2.Y <= bullet_1.Y + 2 && user1r2.Y >= bullet_1.Y - 2) && (user1r2.X <= bullet_1.X + 3 && user1r2.X >= bullet_1.X - 1)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_1.Y + 2 && user2r2.Y >= bullet_1.Y - 2) && (user2r2.X <= bullet_1.X + 3 && user2r2.X >= bullet_1.X - 1)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
            else {
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }

        }
        else if (round2_matrix[bullet_1.Y][bullet_1.X + 1] == (char)186) {
            if (bombuser1 >= 0) {
                round2_matrix[bullet_1.Y][bullet_1.X + 1] = ' ';
                if ((user1r2.Y <= bullet_1.Y + 1 && user1r2.Y >= bullet_1.Y - 1) && (user1r2.X <= bullet_1.X + 2 && user1r2.X >= bullet_1.X)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_1.Y + 1 && user2r2.Y >= bullet_1.Y - 1) && (user2r2.X <= bullet_1.X + 2 && user2r2.X >= bullet_1.X)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
            else bullet1 = left;
        }
        else if (round2_matrix[bullet_1.Y][bullet_1.X + 1] == ' ' || round2_matrix[bullet_1.Y][bullet_1.X + 1] == '@') {
            bullet_1.X += 1;
            if (bullet_1.Y == user1r2.Y && bullet_1.X == user1r2.X) {
                if (gunuser1 >= 0)livesuser1 -= 2;
                else livesuser1 -= 1;
                user1r2 = user1r2_fix;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
            if (bullet_1.Y == user2r2.Y && bullet_1.X == user2r2.X) {
                if (gunuser1 >= 0)livesuser2 -= 2;
                else livesuser2 -= 1;
                user2r2 = user2r2_fix;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
        }
    }
    if (bullet1 == left) {
        if (round2_matrix[bullet_1.Y][bullet_1.X - 1] == (char)219) {
            if (ghostuser1 >= 0) {
                if (bullet_1.X - 1 == 0) {
                    bullet1 = stable;
                    bullet_1.X = -1;
                    bullet_1.Y = -1;
                }
                else bullet_1.X -= 1;
            }
            else if (bombuser1 >= 0) {
                if ((user1r2.Y <= bullet_1.Y + 2 && user1r2.Y >= bullet_1.Y - 2) && (user1r2.X <= bullet_1.X + 1 && user1r2.X >= bullet_1.X - 3)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_1.Y + 2 && user2r2.Y >= bullet_1.Y - 2) && (user2r2.X <= bullet_1.X + 1 && user2r2.X >= bullet_1.X - 3)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
            else {
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
        }
        else if (round2_matrix[bullet_1.Y][bullet_1.X - 1] == (char)186) {
            if (bombuser1 >= 0) {
                round2_matrix[bullet_1.Y][bullet_1.X - 1] = ' ';
                if ((user1r2.Y <= bullet_1.Y + 1 && user1r2.Y >= bullet_1.Y - 1) && (user1r2.X <= bullet_1.X && user1r2.X >= bullet_1.X - 2)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_1.Y + 1 && user2r2.Y >= bullet_1.Y - 1) && (user2r2.X <= bullet_1.X && user2r2.X >= bullet_1.X - 2)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet1 = stable;
                bullet_1.X = -1;
                bullet_1.Y = -1;
            }
            else bullet1 = right;
        }
        else if (round2_matrix[bullet_1.Y][bullet_1.X - 1] == ' ' || round2_matrix[bullet_1.Y][bullet_1.X - 1] == '@') {
            bullet_1.X -= 1;
            if (bullet_1.Y == user1r2.Y && bullet_1.X == user1r2.X) {
                if (gunuser1 >= 0)livesuser1 -= 2;
                else livesuser1 -= 1;
                user1r2 = user1r2_fix;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
            if (bullet_1.Y == user2r2.Y && bullet_1.X == user2r2.X) {
                if (gunuser1 >= 0)livesuser2 -= 2;
                else livesuser2 -= 1;
                user2r2 = user2r2_fix;
                bullet_1.X = -1;
                bullet_1.Y = -1;
                bullet1 = stable;
            }
        }
    }
}
void moveuser1r2() {
    if (keyPressed('W') && (user1r2.Y - 1 > 0)) {
        if (bullet1 == stable) bullet1 = up;
        bullet1_a = up;
        int move = 0;
        if (round2_matrix[user1r2.Y - 1][user1r2.X] == ' ') {
            user1r2 = moveDirection(user1r2, 0, -1);
            move = 1;
            if (user1r2.X == 35 && user1r2.Y == 9 && heartr2.X != 0) {
                heartr2.X = 0;
                livesuser1 += 5;
            }
            else if (user1r2.X == 63 && user1r2.Y == 17) {
                user1r2.X = 9;
                user1r2.Y = 17;
            }//portals
            else if (user1r2.X == 9 && user1r2.Y == 17) {
                user1r2.X = 63;
                user1r2.Y = 17;
            }//portals
            else if (user1r2.X == 51 && user1r2.Y == 7 && gun_up1.X != 0) {
                gunuser1 += 5;
                gun_up1.X = 0;
            }
            else if (user1r2.X == 20 && user1r2.Y == 7 && gun_up2.X != 0) {
                gunuser1 += 5;
                gun_up2.X = 0;
            }
            else if (user1r2.X == 35 && user1r2.Y == 13 && bomb.X != 0) {
                bombuser1 = 1;
                bomb.X = 0;
            }
            else if (user1r2.X == 35 && user1r2.Y == 2 && ghost.X != 0) {
                ghostuser1 = 7;
                ghost.X = 0;
            }
        }
        if (ghostuser1 >= 0) {
            if (round2_matrix[user1r2.Y - 1][user1r2.X] == (char)219 && move != 1)
                user1r2 = moveDirection(user1r2, 0, -1);
            --ghostuser1;
        }
        if (round2_matrix[user1r2.Y - 1][user1r2.X] == '@') {
            user1r2 = user1r2_fix;
        }
    }
    if (keyPressed('S') && (user1r2.Y + 1 < 19)) {
        if (bullet1 == stable) bullet1 = down;
        bullet1_a = down;
        int move = 0;
        if (round2_matrix[user1r2.Y + 1][user1r2.X] == ' ') {
            user1r2 = moveDirection(user1r2, 0, 1);
            move = 1;
            if (user1r2.X == 35 && user1r2.Y == 9 && heartr2.X != 0) {
                heartr2.X = 0;
                livesuser1 += 5;
            }
            else if (user1r2.X == 63 && user1r2.Y == 17) {
                user1r2.X = 9;
                user1r2.Y = 17;
            }//portals
            else if (user1r2.X == 9 && user1r2.Y == 17) {
                user1r2.X = 63;
                user1r2.Y = 17;
            }//portals
            else if (user1r2.X == 51 && user1r2.Y == 7 && gun_up1.X != 0) {
                gunuser1 += 5;
                gun_up1.X = 0;
            }
            else if (user1r2.X == 20 && user1r2.Y == 7 && gun_up2.X != 0) {
                gunuser1 += 5;
                gun_up2.X = 0;
            }
            else if (user1r2.X == 35 && user1r2.Y == 13 && bomb.X != 0) {
                bombuser1 = 1;
                bomb.X = 0;
            }
            else if (user1r2.X == 35 && user1r2.Y == 2 && ghost.X != 0) {
                ghostuser1 = 7;
                ghost.X = 0;
            }
        }
        if (ghostuser1 >= 0) {
            if (round2_matrix[user1r2.Y + 1][user1r2.X] == (char)219 && move != 1)
                user1r2 = moveDirection(user1r2, 0, 1);
            --ghostuser1;
        }
        if (round2_matrix[user1r2.Y + 1][user1r2.X] == '@') {
            user1r2 = user1r2_fix;
        }

    }
    if (keyPressed('A') && (user1r2.X - 1 > 0)) {
        if (bullet1 == stable) bullet1 = left;
        bullet1_a = left;
        int move = 0;
        if (round2_matrix[user1r2.Y][user1r2.X - 1] == ' ') {
            user1r2 = moveDirection(user1r2, -1, 0);
            move = 1;
            if (user1r2.X == 35 && user1r2.Y == 9 && heartr2.X != 0) {
                heartr2.X = 0;
                livesuser1 += 5;
            }
            else if (user1r2.X == 63 && user1r2.Y == 17) {
                user1r2.X = 9;
                user1r2.Y = 17;
            }//portals
            else if (user1r2.X == 9 && user1r2.Y == 17) {
                user1r2.X = 63;
                user1r2.Y = 17;
            }//portals
            else if (user1r2.X == 51 && user1r2.Y == 7 && gun_up1.X != 0) {
                gunuser1 += 5;
                gun_up1.X = 0;
            }
            else if (user1r2.X == 20 && user1r2.Y == 7 && gun_up2.X != 0) {
                gunuser1 += 5;
                gun_up2.X = 0;
            }
            else if (user1r2.X == 35 && user1r2.Y == 13 && bomb.X != 0) {
                bombuser1 = 1;
                bomb.X = 0;
            }
            else if (user1r2.X == 35 && user1r2.Y == 2 && ghost.X != 0) {
                ghostuser1 = 7;
                ghost.X = 0;
            }
        }
        if (ghostuser1 >= 0) {
            if (round2_matrix[user1r2.Y][user1r2.X - 1] == (char)219 && move != 1)
                user1r2 = moveDirection(user1r2, -1, 0);
            --ghostuser1;
        }
        if (round2_matrix[user1r2.Y][user1r2.X - 1] == '@') {
            user1r2 = user1r2_fix;
        }
    }
    if (keyPressed('D') && (user1r2.X + 1 < 69)) {
        if (bullet1 == stable) bullet1 = right;
        bullet1_a = right;
        int move = 0;
        if (round2_matrix[user1r2.Y][user1r2.X + 1] == ' ') {
            user1r2 = moveDirection(user1r2, 1, 0);
            move = 1;
            if (user1r2.X == 35 && user1r2.Y == 9 && heartr2.X != 0) {
                heartr2.X = 0;
                livesuser1 += 5;
            }
            else if (user1r2.X == 63 && user1r2.Y == 17) {
                user1r2.X = 9;
                user1r2.Y = 17;
            }//portals
            else if (user1r2.X == 9 && user1r2.Y == 17) {
                user1r2.X = 63;
                user1r2.Y = 17;
            }//portals
            else if (user1r2.X == 51 && user1r2.Y == 7 && gun_up1.X != 0) {
                gunuser1 += 5;
                gun_up1.X = 0;
            }
            else if (user1r2.X == 20 && user1r2.Y == 7 && gun_up2.X != 0) {
                gunuser1 += 5;
                gun_up2.X = 0;
            }
            else if (user1r2.X == 35 && user1r2.Y == 13 && bomb.X != 0) {
                bombuser1 = 1;
                bomb.X = 0;
            }
            else if (user1r2.X == 35 && user1r2.Y == 2 && ghost.X != 0) {
                ghostuser1 = 7;
                ghost.X = 0;
            }
        }
        if (ghostuser1 >= 0) {
            if (round2_matrix[user1r2.Y][user1r2.X + 1] == (char)219 && move != 1)
                user1r2 = moveDirection(user1r2, 1, 0);
            --ghostuser1;
        }
        if (round2_matrix[user1r2.Y][user1r2.X + 1] == '@') {
            user1r2 = user1r2_fix;
        }

    }
    movebullet1r2();
}
void movebullet2r2() {
    if (keyPressed('N')) {
        bullet_2 = user2r2;
        if (bullet2 != stable)
            bullet2 = bullet2_a;
        else bullet2 = bullet2_a;
        if (ghostuser2 >= 0) --ghostuser2;
        if (gunuser2 >= 0) --gunuser2;
        if (bombuser2 >= 0) --bombuser2;
    }
    if (bullet2 == up) {
        if (round2_matrix[bullet_2.Y - 1][bullet_2.X] == (char)219) {
            if (ghostuser2 >= 0) {
                if (bullet_2.Y - 1 == 0) {
                    bullet2 = stable;
                    bullet_2.X = -1;
                    bullet_2.Y = -1;
                }
                else bullet_2.Y -= 1;
            }
            else if (bombuser2 >= 0) {
                if ((user1r2.Y <= bullet_2.Y + 1 && user1r2.Y >= bullet_2.Y - 3) && (user1r2.X <= bullet_2.X + 2 && user1r2.X >= bullet_2.X - 2)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_2.Y + 1 && user2r2.Y >= bullet_2.Y - 3) && (user2r2.X <= bullet_2.X + 2 && user2r2.X >= bullet_2.X - 2)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
            else {
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }

        }
        else if (round2_matrix[bullet_2.Y - 1][bullet_2.X] == (char)186) {
            if (bombuser2 >= 0) {
                round2_matrix[bullet_2.Y - 1][bullet_2.X] = ' ';
                if ((user1r2.Y <= bullet_2.Y && user1r2.Y >= bullet_2.Y - 2) && (user1r2.X <= bullet_2.X + 1 && user1r2.X >= bullet_2.X - 1)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_2.Y && user2r2.Y >= bullet_2.Y - 2) && (user2r2.X <= bullet_2.X + 1 && user2r2.X >= bullet_2.X - 1)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
            else bullet2 = down;
        }
        else if (round2_matrix[bullet_2.Y - 1][bullet_2.X] == ' ' || round2_matrix[bullet_2.Y - 1][bullet_2.X] == '@') {
            bullet_2.Y -= 1;
            if (bullet_2.Y == user1r2.Y && bullet_2.X == user1r2.X) {
                if (gunuser2 >= 0)livesuser1 -= 2;
                else livesuser1 -= 1;
                user1r2 = user1r2_fix;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
            if (bullet_2.Y == user2r2.Y && bullet_2.X == user2r2.X) {
                if (gunuser2 >= 0)livesuser2 -= 2;
                else livesuser2 -= 1;
                user2r2 = user2r2_fix;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
        }
    }
    if (bullet2 == down) {
        if (round2_matrix[bullet_2.Y + 1][bullet_2.X] == (char)219) {
            if (ghostuser2 >= 0) {
                if (bullet_2.Y + 1 == 19) {
                    bullet2 = stable;
                    bullet_2.X = -1;
                    bullet_2.Y = -1;
                }
                else bullet_2.Y += 1;
            }
            else if (bombuser2 >= 0) {
                if ((user1r2.Y <= bullet_2.Y + 3 && user1r2.Y >= bullet_2.Y - 1) && (user1r2.X <= bullet_2.X + 2 && user1r2.X >= bullet_2.X - 2)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_2.Y + 3 && user2r2.Y >= bullet_2.Y - 1) && (user2r2.X <= bullet_2.X + 2 && user2r2.X >= bullet_2.X - 2)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
            else {
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
        }
        else if (round2_matrix[bullet_2.Y + 1][bullet_2.X] == (char)186) {
            if (bombuser2 >= 0) {
                round2_matrix[bullet_2.Y + 1][bullet_2.X] = ' ';
                if ((user1r2.Y <= bullet_2.Y + 2 && user1r2.Y >= bullet_2.Y) && (user1r2.X <= bullet_2.X + 1 && user1r2.X >= bullet_2.X - 1)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_2.Y + 2 && user2r2.Y >= bullet_2.Y) && (user2r2.X <= bullet_2.X + 1 && user2r2.X >= bullet_2.X - 1)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
            else bullet2 = up;
        }
        else if (round2_matrix[bullet_2.Y + 1][bullet_2.X] == ' ' || round2_matrix[bullet_2.Y + 1][bullet_2.X] == '@') {
            bullet_2.Y += 1;
            if (bullet_2.Y == user1r2.Y && bullet_2.X == user1r2.X) {
                if (gunuser2 >= 0)livesuser1 -= 2;
                else livesuser1 -= 1;
                user1r2 = user1r2_fix;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
            if (bullet_2.Y == user2r2.Y && bullet_2.X == user2r2.X) {
                if (gunuser2 >= 0)livesuser2 -= 2;
                else livesuser2 -= 1;
                user2r2 = user2r2_fix;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
        }
    }
    if (bullet2 == right) {
        if (round2_matrix[bullet_2.Y][bullet_2.X + 1] == (char)219) {
            if (ghostuser2 >= 0) {
                if (bullet_2.X + 1 == 69) {
                    bullet2 = stable;
                    bullet_2.X = -1;
                    bullet_2.Y = -1;
                }
                else bullet_2.X += 1;
            }
            else if (bombuser2 >= 0) {
                if ((user1r2.Y <= bullet_2.Y + 2 && user1r2.Y >= bullet_2.Y - 2) && (user1r2.X <= bullet_2.X + 3 && user1r2.X >= bullet_2.X - 1)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_2.Y + 2 && user2r2.Y >= bullet_2.Y - 2) && (user2r2.X <= bullet_2.X + 3 && user2r2.X >= bullet_2.X - 1)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
            else {
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
        }
        else if (round2_matrix[bullet_2.Y][bullet_2.X + 1] == (char)186) {
            if (bombuser2 >= 0) {
                round2_matrix[bullet_2.Y][bullet_2.X + 1] = ' ';
                if ((user1r2.Y <= bullet_2.Y + 1 && user1r2.Y >= bullet_2.Y - 1) && (user1r2.X <= bullet_2.X + 2 && user1r2.X >= bullet_2.X)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_2.Y + 1 && user2r2.Y >= bullet_2.Y - 1) && (user2r2.X <= bullet_2.X + 2 && user2r2.X >= bullet_2.X)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
            else bullet2 = left;
        }
        else if (round2_matrix[bullet_2.Y][bullet_2.X + 1] == ' ' || round2_matrix[bullet_2.Y][bullet_2.X + 1] == '@') {
            bullet_2.X += 1;
            if (bullet_2.Y == user1r2.Y && bullet_2.X == user1r2.X) {
                if (gunuser2 >= 0)livesuser1 -= 2;
                else livesuser1 -= 1;
                user1r2 = user1r2_fix;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
            if (bullet_2.Y == user2r2.Y && bullet_2.X == user2r2.X) {
                if (gunuser2 >= 0)livesuser2 -= 2;
                else livesuser2 -= 1;
                user2r2 = user2r2_fix;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
        }
    }
    if (bullet2 == left) {
        if (round2_matrix[bullet_2.Y][bullet_2.X - 1] == (char)219) {
            if (ghostuser2 >= 0) {
                if (bullet_2.X - 1 == 0) {
                    bullet2 = stable;
                    bullet_2.X = -1;
                    bullet_2.Y = -1;
                }
                else bullet_2.X -= 1;
            }
            else if (bombuser2 >= 0) {
                if ((user1r2.Y <= bullet_2.Y + 2 && user1r2.Y >= bullet_2.Y - 2) && (user1r2.X <= bullet_2.X + 1 && user1r2.X >= bullet_2.X - 3)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_2.Y + 2 && user2r2.Y >= bullet_2.Y - 2) && (user2r2.X <= bullet_2.X + 1 && user2r2.X >= bullet_2.X - 3)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
            else {
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
        }
        else if (round2_matrix[bullet_2.Y][bullet_2.X - 1] == (char)186) {
            if (bombuser2 >= 0) {
                round2_matrix[bullet_2.Y][bullet_2.X - 1] = ' ';
                if ((user1r2.Y <= bullet_2.Y + 1 && user1r2.Y >= bullet_2.Y - 1) && (user1r2.X <= bullet_2.X && user1r2.X >= bullet_2.X - 2)) {
                    user1r2 = user1r2_fix;
                    --livesuser1;
                }
                if ((user2r2.Y <= bullet_2.Y + 1 && user2r2.Y >= bullet_2.Y - 1) && (user2r2.X <= bullet_2.X && user2r2.X >= bullet_2.X - 2)) {
                    user2r2 = user2r2_fix;
                    --livesuser2;
                }
                bullet2 = stable;
                bullet_2.X = -1;
                bullet_2.Y = -1;
            }
            else bullet2 = right;
        }
        else if (round2_matrix[bullet_2.Y][bullet_2.X - 1] == ' ' || round2_matrix[bullet_2.Y][bullet_2.X - 1] == '@') {
            bullet_2.X -= 1;
            if (bullet_2.Y == user1r2.Y && bullet_2.X == user1r2.X) {
                if (gunuser2 >= 0)livesuser1 -= 2;
                else livesuser1 -= 1;
                user1r2 = user1r2_fix;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
            if (bullet_2.Y == user2r2.Y && bullet_2.X == user2r2.X) {
                if (gunuser2 >= 0)livesuser2 -= 2;
                else livesuser2 -= 1;
                user2r2 = user2r2_fix;
                bullet_2.X = -1;
                bullet_2.Y = -1;
                bullet2 = stable;
            }
        }
    }
}
void moveuser2r2() {
    if (keyPressed('I') && (user2r2.Y - 1 > 0)) {
        if (bullet2 == stable) bullet2 = up;
        bullet2_a = up;
        int move = 0;
        if (round2_matrix[user2r2.Y - 1][user2r2.X] == ' ') {
            user2r2 = moveDirection(user2r2, 0, -1);
            move = 1;
            if (user2r2.X == 35 && user2r2.Y == 9 && heartr2.X != 0) {
                heartr2.X = 0;
                livesuser2 += 5;
            }
            else if (user2r2.X == 63 && user2r2.Y == 17) {
                user2r2.X = 9;
                user2r2.Y = 17;
            }//portals
            else if (user2r2.X == 9 && user2r2.Y == 17) {
                user2r2.X = 63;
                user2r2.Y = 17;
            }//portals
            else if (user2r2.X == 51 && user2r2.Y == 7 && gun_up1.X != 0) {
                gunuser2 += 5;
                gun_up1.X = 0;
            }
            else if (user2r2.X == 20 && user2r2.Y == 7 && gun_up2.X != 0) {
                gunuser2 += 5;
                gun_up2.X = 0;
            }
            else if (user2r2.X == 35 && user2r2.Y == 13 && bomb.X != 0) {
                bombuser2 = 1;
                bomb.X = 0;
            }
            else if (user2r2.X == 35 && user2r2.Y == 2 && ghost.X != 0) {
                ghostuser2 = 7;
                ghost.X = 0;
            }
        }
        if (ghostuser2 >= 0) {
            if (round2_matrix[user2r2.Y - 1][user2r2.X] == (char)219 && move != 1)
                user2r2 = moveDirection(user2r2, 0, -1);
            --ghostuser2;
        }
        if (round2_matrix[user2r2.Y - 1][user2r2.X] == '@') {
            user2r2 = user2r2_fix;
        }
    }
    if (keyPressed('K') && (user2r2.Y + 1 < 19)) {
        if (bullet2 == stable) bullet2 = down;
        bullet2_a = down;
        int move = 0;
        if (round2_matrix[user2r2.Y + 1][user2r2.X] == ' ') {
            user2r2 = moveDirection(user2r2, 0, +1);
            move = 1;
            if (user2r2.X == 35 && user2r2.Y == 9 && heartr2.X != 0) {
                heartr2.X = 0;
                livesuser2 += 5;
            }
            else if (user2r2.X == 63 && user2r2.Y == 17) {
                user2r2.X = 9;
                user2r2.Y = 17;
            }//portals
            else if (user2r2.X == 9 && user2r2.Y == 17) {
                user2r2.X = 63;
                user2r2.Y = 17;
            }//portals
            else if (user2r2.X == 51 && user2r2.Y == 7 && gun_up1.X != 0) {
                gunuser2 += 5;
                gun_up1.X = 0;
            }
            else if (user2r2.X == 20 && user2r2.Y == 7 && gun_up2.X != 0) {
                gunuser2 += 5;
                gun_up2.X = 0;
            }
            else if (user2r2.X == 35 && user2r2.Y == 13 && bomb.X != 0) {
                bombuser2 = 1;
                bomb.X = 0;
            }
            else if (user2r2.X == 35 && user2r2.Y == 2 && ghost.X != 0) {
                ghostuser2 = 7;
                ghost.X = 0;
            }
        }
        if (ghostuser2 >= 0) {
            if (round2_matrix[user2r2.Y + 1][user2r2.X] == (char)219 && move != 1)
                user2r2 = moveDirection(user2r2, 0, 1);
            --ghostuser2;
        }
        if (round2_matrix[user2r2.Y + 1][user2r2.X] == '@') {
            user2r2 = user2r2_fix;
        }
    }
    if (keyPressed('J') && (user2r2.X - 1 > 0)) {
        if (bullet2 == stable) bullet2 = left;
        bullet2_a = left;
        int move = 0;
        if (round2_matrix[user2r2.Y][user2r2.X - 1] == ' ') {
            user2r2 = moveDirection(user2r2, -1, 0);
            move = 1;
            if (user2r2.X == 35 && user2r2.Y == 9 && heartr2.X != 0) {
                heartr2.X = 0;
                livesuser2 += 5;
            }
            else if (user2r2.X == 63 && user2r2.Y == 17) {
                user2r2.X = 9;
                user2r2.Y = 17;
            }//portals
            else if (user2r2.X == 9 && user2r2.Y == 17) {
                user2r2.X = 63;
                user2r2.Y = 17;
            }//portals
            else if (user2r2.X == 51 && user2r2.Y == 7 && gun_up1.X != 0) {
                gunuser2 += 5;
                gun_up1.X = 0;
            }
            else if (user2r2.X == 20 && user2r2.Y == 7 && gun_up2.X != 0) {
                gunuser2 += 5;
                gun_up2.X = 0;
            }
            else if (user2r2.X == 35 && user2r2.Y == 13 && bomb.X != 0) {
                bombuser2 = 1;
                bomb.X = 0;
            }
            else if (user2r2.X == 35 && user2r2.Y == 2 && ghost.X != 0) {
                ghostuser2 = 7;
                ghost.X = 0;
            }
        }
        if (ghostuser2 >= 0) {
            if (round2_matrix[user2r2.Y][user2r2.X - 1] == (char)219 && move != 1)
                user2r2 = moveDirection(user2r2, -1, 0);
            --ghostuser2;
        }
        if (round2_matrix[user2r2.Y][user2r2.X - 1] == '@') {
            user2r2 = user2r2_fix;
        }
    }
    if (keyPressed('L') && (user2r2.X + 1 < 69)) {
        if (bullet2 == stable) bullet2 = right;
        bullet2_a = right;
        int move = 0;
        if (round2_matrix[user2r2.Y][user2r2.X + 1] == ' ') {
            user2r2 = moveDirection(user2r2, 1, 0);
            move = 1;
            if (user2r2.X == 35 && user2r2.Y == 9 && heartr2.X != 0) {
                heartr2.X = 0;
                livesuser2 += 5;
            }
            else if (user2r2.X == 63 && user2r2.Y == 17) {
                user2r2.X = 9;
                user2r2.Y = 17;
            }//portals
            else if (user2r2.X == 9 && user2r2.Y == 17) {
                user2r2.X = 63;
                user2r2.Y = 17;
            }//portals
            else if (user2r2.X == 51 && user2r2.Y == 7 && gun_up1.X != 0) {
                gunuser2 += 5;
                gun_up1.X = 0;
            }
            else if (user2r2.X == 20 && user2r2.Y == 7 && gun_up2.X != 0) {
                gunuser2 += 5;
                gun_up2.X = 0;
            }
            else if (user2r2.X == 35 && user2r2.Y == 13 && bomb.X != 0) {
                bombuser2 = 1;
                bomb.X = 0;
            }
            else if (user2r2.X == 35 && user2r2.Y == 2 && ghost.X != 0) {
                ghostuser2 = 7;
                ghost.X = 0;
            }
        }
        if (ghostuser2 >= 0) {
            if (round2_matrix[user2r2.Y][user2r2.X + 1] == (char)219 && move != 1)
                user2r2 = moveDirection(user2r2, 1, 0);
            --ghostuser2;
        }
        if (round2_matrix[user2r2.Y][user2r2.X + 1] == '@') {
            user2r2 = user2r2_fix;
        }
    }
    movebullet2r2();
}
void gamemode() {
    map1();
    map2();
    DWORD bytes_written;
    DWORD bytes_written2;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdout2 = GetStdHandle(STD_OUTPUT_HANDLE);
    //_setmode(_fileno(stdout), _O_U16TEXT);
    while (1) {
        for (int y = 0; y < 15; y++) {
            COORD place;
            place.X = 0;
            place.Y = y;
            WriteConsoleOutputCharacterA(hStdout, round1_matrix[y], 60, place, &bytes_written);
        }
        moveuser1();
        moveuser2();
        if (heart.X != 0) {
            gotoxy(29, 3);
            printf("\033[1;31m%c\033[0m", 3);
            gotoxy(0, 0);
        }
        if (bullet_1.X != -1) {
            gotoxy(bullet_1.X, bullet_1.Y);
            printf("*");
        }
        gotoxy(user1r1.X, user1r1.Y);
        WriteConsoleW(hStdout, L"\U00002776", 1, &bytes_written, NULL);
        //printf("1");
        gotoxy(user2r1.X, user2r1.Y);
        WriteConsoleW(hStdout, L"\U00002777", 1, &bytes_written, NULL);
        //printf("2");
        if (bullet_2.X != -1) {
            gotoxy(bullet_2.X, bullet_2.Y);
            printf("*");
        }
        gotoxy(53, 11);
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN);
        WriteConsoleW(hStdout, L"\U0000058D", 1, &bytes_written, NULL);
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        gotoxy(8, 11);
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN);
        WriteConsoleW(hStdout, L"\U0000058D", 1, &bytes_written, NULL);
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        gotoxy(0, 15);
        printf("ROUND 1 :");
        gotoxy(3, 17);
        printf("Player1:%s     Player1 lives: %d      Rounds Won: %d", ui.user1, livesuser1, round_won1);
        gotoxy(3, 18);
        printf("Player2:%s     Player2 lives: %d      Rounds Won: %d", ui.user2, livesuser2, round_won2);
        gotoxy(0, 0);
        if (livesuser1 == 0) {
            ++round_won2;
            bullet_1.X = -1;
            bullet_1.Y = -1;
            bullet_2.X = -1;
            bullet_2.Y = -1;
            bullet1 = stable;
            bullet2 = stable;
            heart.X = 29;
            user1r1 = user1r1_fix;
            user2r1 = user2r1_fix;
            livesuser1 = result;
            livesuser2 = result;
            clear();
            gotoxy(30, 8);
            printf("\033[1;32m%s WON this round\033[0m", ui.user2);
            Sleep(3000);
            break;
        }
        if (livesuser2 == 0) {
            ++round_won1;
            bullet_1.X = -1;
            bullet_1.Y = -1;
            bullet_2.X = -1;
            bullet_2.Y = -1;
            bullet1 = stable;
            bullet2 = stable;
            heart.X = 29;
            user1r1 = user1r1_fix;
            user2r1 = user2r1_fix;
            livesuser1 = result;
            livesuser2 = result;
            clear();
            gotoxy(30, 8);
            printf("\033[1;32m%s WON this round\033[0m", ui.user1);
            Sleep(3000);
            break;
        }
        Sleep(100);
    }
    clear();
    while (1) {
        for (int y = 0; y < 20; y++) {
            COORD place;
            place.X = 0;
            place.Y = y;
            WriteConsoleOutputCharacterA(hStdout2, round2_matrix[y], 70, place, &bytes_written2);
        }
        moveuser1r2();
        moveuser2r2();
        if (heartr2.X != 0) {
            gotoxy(35, 9);
            printf("\033[1;31m%c\033[0m", 3);
        }
        if (bomb.X != 0) {
            gotoxy(35, 13);
            printf("\033[1;32m%c\033[0m", 162);
        }
        if (gun_up1.X != 0) {
            gotoxy(51, 7);
            //printf("\033[1;35m#\033[0m");
            SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN);
            WriteConsoleW(hStdout2, L"\U0001F5F2", 2, &bytes_written2, NULL);
            SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        if (gun_up2.X != 0) {
            gotoxy(20, 7);
            //printf("\033[1;35m#\033[0m");
            SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN);
            WriteConsoleW(hStdout2, L"\U0001F5F2", 2, &bytes_written2, NULL);
            SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        if (ghost.X != 0) {
            gotoxy(35, 2);
            //printf("\033[1;37m$\033[0m");
            WriteConsoleW(hStdout2, L"\U0001F47B", 2, &bytes_written2, NULL);
        }
        gotoxy(63, 17);//portal
        SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN);
        WriteConsoleW(hStdout2, L"\U0000058D", 1, &bytes_written2, NULL);
        SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        gotoxy(9, 17);//portal
        SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN);
        WriteConsoleW(hStdout2, L"\U0000058D", 1, &bytes_written2, NULL);
        SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        gotoxy(user1r2.X, user1r2.Y);
        WriteConsoleW(hStdout2, L"\U00002776", 1, &bytes_written2, NULL);
        //SetConsoleTextAttribute(hStdout2, FOREGROUND_RED);
        //WriteConsoleW(hStdout2, L"\U00002733", 1, &bytes_written2, NULL);
        //SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        if (bullet_1.X != -1) {
            gotoxy(bullet_1.X, bullet_1.Y);
            printf("*");
        }
        gotoxy(user2r2.X, user2r2.Y);
        WriteConsoleW(hStdout2, L"\U00002777", 1, &bytes_written2, NULL);
        //SetConsoleTextAttribute(hStdout2, FOREGROUND_BLUE);
        //WriteConsoleW(hStdout2, L"\U00002B1C", 1, &bytes_written2, NULL);
        //SetConsoleTextAttribute(hStdout2, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        if (bullet_2.X != -1) {
            gotoxy(bullet_2.X, bullet_2.Y);
            printf("*");
        }
        gotoxy(0, 20);
        printf("ROUND2 :");
        gotoxy(5, 22);
        printf("Player1:%s    Lives of player1: %d  Rounds Won: %d", ui.user1, livesuser1, round_won1);
        gotoxy(5, 23);
        printf("Player2:%s    Lives of player2: %d  Rounds Won: %d", ui.user2, livesuser2, round_won2);
        gotoxy(0, 0);
        Sleep(100);
        if (livesuser1 == 0) {
            ++round_won2;
            bullet_1.X = -1;
            bullet_1.Y = -1;
            bullet_2.X = -1;
            bullet_2.Y = -1;
            bullet1 = stable;
            bullet2 = stable;
            heartr2.X = 35;
            bomb.X = 35;
            gun_up1.X = 51;
            gun_up2.X = 20;
            ghost.X = 35;
            gunuser1 = 0;
            gunuser2 = 0;
            ghostuser1 = 0;
            ghostuser2 = 0;
            bombuser1 = 0;
            bombuser2 = 0;
            user1r2 = user1r2_fix;
            user2r2 = user2r2_fix;
            livesuser1 = 0;
            livesuser2 = 0;
            clear();
            gotoxy(35, 10);
            if (round_won2 > round_won1) {
                printf("\033[1;32m%s is the WINNER\033[0m", ui.user2);
                mr.game_status = 2;
            }
            else {
                printf("\033[1;37mTied Match\033[0m");
                gotoxy(35, 11);
                printf("\033[1;37m%s = %s\033[0m", ui.user1, ui.user2);
                mr.game_status = 0;
            }
            round_won1 = 0;
            round_won2 = 0;
            Sleep(3000);
            break;
        }
        if (livesuser2 == 0) {
            ++round_won1;
            bullet_1.X = -1;
            bullet_1.Y = -1;
            bullet_2.X = -1;
            bullet_2.Y = -1;
            bullet1 = stable;
            bullet2 = stable;
            heartr2.X = 35;
            bomb.X = 35;
            gun_up1.X = 51;
            gun_up2.X = 20;
            ghost.X = 35;
            gunuser1 = 0;
            gunuser2 = 0;
            ghostuser1 = 0;
            ghostuser2 = 0;
            bombuser1 = 0;
            bombuser2 = 0;
            user1r2 = user1r2_fix;
            user2r2 = user2r2_fix;
            livesuser1 = 0;
            livesuser2 = 0;
            clear();
            gotoxy(35, 10);
            if (round_won1 > round_won2) {
                printf("\033[1;32m%s is the WINNER\033[0m", ui.user1);
                mr.game_status = 1;
            }
            else {
                printf("\033[1;37mTied Match\033[0m");
                gotoxy(35, 11);
                printf("\033[1;37m%s = %s\033[0m", ui.user1, ui.user2);
                mr.game_status = 0;
            }
            round_won1 = 0;
            round_won2 = 0;
            Sleep(3000);
            break;
        }
    }
    //strcpy(mr.player1, ui.user1);
    //strcpy(mr.player2, ui.user2);
}
void flush_stdin() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}
int game = 0;
void game_menu() {
    while (1) {
        clear();
        printf("Your ID:%s\n  Menu:\n\n  1)Start\n  2)Changing Information\n  3)History\n  4)Return to Login Page\nType correct Number:", ui.user1);
        if (game == 1) {
            /*flush_stdin();
            game = 0;
            continue;*/
            char c;
            do {
                c = _getche();
                if (c == '1' || c == '2' || c == '3' || c == '4')break;
                printf("\b \b");
            } while (c != '\r' && c != EOF);
            game = 0;
            continue;
        }
        int input = get_digit();
        if (input == 1) {
            while (1) {
                clear();
                printf("1)Login for User2\n2)Sign up for User2\nType correct number:");
                int condition = get_digit();
                if (condition == 1) {
                    FILE* user_info = fopen("data.txt", "r");
                    clear();
                    while (1) {
                        printf("Username:");
                        scanf_string(si.username);
                        fseek(user_info, 0, SEEK_SET);
                        if (!find_user(si.username, fi, user_info)) {
                            printf("\nUsername doesn't exist");
                            Sleep(2000);
                            clear();
                            continue;
                        }
                        else break;
                    }
                    while (1) {
                        clear();
                        fseek(user_info, 0, SEEK_SET);
                        while (fread(&fi, sizeof(file_id), 1, user_info)) {
                            if (strcmp(fi.username, si.username) == 0)break;
                        }
                        printf("Username: %s", si.username);
                        printf("\nPassword:");
                        star_password(si.password);
                        if (strncmp(fi.password, si.password, strlen(fi.password)) != 0) {
                            printf("\nyour password is incorrect");
                            Sleep(2000);
                            continue;
                        }
                        else break;
                    }
                    fclose(user_info);
                    strcpy(ui.user2, si.username);
                    clear();
                    printf("Enter lives of each user in each round(1-9):");
                    result = -1;
                    while (result == -1)
                    {
                        char inp = getchar();
                        if (inp >= '1' && inp <= '9')
                            result = inp - '0';
                    }
                    livesuser1 = result;
                    livesuser2 = result;
                    clear();
                    gamemode();
                    strcpy(mr.player1, ui.user1);
                    strcpy(mr.player2, ui.user2);
                    FILE* match_info = fopen("matchdata.txt", "a");
                    fwrite(&mr, sizeof(match_result), 1, match_info);
                    fclose(match_info);
                    //flush_stdin();
                    //flush_stdin();
                    game = 1;
                    break;
                }
                else if (condition == 2) {
                    clear();
                    Sign_up();
                }
            }
        }
        else if (input == 2) {
            while (1) {
                clear();
                printf("Do you want to change which one:\n1)Username\n2)Password\n3)Email\nEnter correct number:");
                FILE* user_info = fopen("data.txt", "r+");
                fseek(user_info, 0, SEEK_SET);
                /*while (fread(&fi, sizeof(file_id), 1, user_info)) {
                    if (strcmp(fi.username, ui.user1) == 0)break;
                }*/
                int condition = get_digit();
                if (condition == 1) {
                    while (1) {
                        clear();
                        printf("Type your new Username :");
                        scanf_string(si.username);
                        fseek(user_info, 0, SEEK_SET);
                        if (!valid_user(si.username)) {
                            printf("\nYour Username is invalid!");
                            Sleep(2000);
                            continue;
                        }
                        else if (find_user(si.username, fi, user_info)) {
                            printf("\nYour username has already been used\n");
                            Sleep(2000);
                            continue;
                        }
                        else break;
                    }
                    fseek(user_info, 0, SEEK_SET);
                    while (fread(&fi, sizeof(file_id), 1, user_info)) {
                        if (strcmp(fi.username, ui.user1) == 0)break;
                    }
                    strcpy(fi.username, si.username);
                    fseek(user_info, -(int)sizeof(file_id), SEEK_CUR);
                    fwrite(&fi, sizeof(file_id), 1, user_info);
                    fclose(user_info);
                    FILE* match_info = fopen("matchdata.txt", "r+");
                    //fseek(match_info, 0, SEEK_SET);

                    fseek(match_info, 0, SEEK_END);
                    size_t file_size = ftell(match_info);
                    fseek(match_info, 0, SEEK_SET);


                    for (int i = 0; i < file_size / sizeof(match_result); i++) {
                        fseek(match_info, i * sizeof(match_result), SEEK_SET);
                        fread(&mr, sizeof(match_result), 1, match_info);
                        if (strcmp(mr.player1, ui.user1) == 0) {
                            strcpy(mr.player1, si.username);
                            fseek(match_info, i * sizeof(match_result), SEEK_SET);
                            fwrite(&mr, sizeof(match_result), 1, match_info);
                        }
                        else if (strcmp(mr.player2, ui.user1) == 0) {
                            strcpy(mr.player2, si.username);
                            fseek(match_info, i * sizeof(match_result), SEEK_SET);
                            fwrite(&mr, sizeof(match_result), 1, match_info);
                        }

                        //fseek(match_info, -(int)sizeof(match_result), SEEK_CUR);
                        //fseek(match_info, i * sizeof(match_result), SEEK_SET);
                        //fwrite(&mr, sizeof(match_result), 1, match_info);
                    }
                    fclose(match_info);
                    strcpy(ui.user1, si.username);
                    printf("Your Username changed");
                    Sleep(2000);
                    break;
                }
                else if (condition == 2) {
                    char repeat[1000];
                    while (1) {
                        clear();
                        printf("New Password(at least 8 character):");
                        scanf_string(si.password);
                        if (strlen(si.password) < 8) {
                            printf("\nPassword is less than 8 characters");
                            Sleep(1000);
                            clear();
                            continue;
                        }
                        else break;
                    }
                    while (1) {
                        printf("\nType Password again:");
                        scanf_string(repeat);
                        if (strcmp(si.password, repeat) != 0) {
                            printf("\nPassword repetition does not match");
                            Sleep(1000);
                            clear();
                            continue;
                        }
                        else break;
                    }
                    while (fread(&fi, sizeof(file_id), 1, user_info)) {
                        if (strcmp(fi.username, ui.user1) == 0)break;
                    }
                    strcpy(fi.password, si.password);
                    fseek(user_info, -(int)sizeof(file_id), SEEK_CUR);
                    fwrite(&fi, sizeof(file_id), 1, user_info);
                    fclose(user_info);
                    printf("\nYour password changed");
                    Sleep(2000);
                    break;
                }
                else if (condition == 3) {
                    while (1) {
                        clear();
                        printf("Type your Email:");
                        scanf_string(si.email);
                        if (!valid_email(si.email)) {
                            clear();
                            printf("\nYour Email is invalid!");
                            Sleep(2000);
                            continue;
                        }
                        else break;
                    }
                    while (fread(&fi, sizeof(file_id), 1, user_info)) {
                        if (strcmp(fi.username, ui.user1) == 0)break;
                    }
                    strcpy(fi.email, si.email);
                    fseek(user_info, -(int)sizeof(file_id), SEEK_CUR);
                    fwrite(&fi, sizeof(file_id), 1, user_info);
                    fclose(user_info);
                    printf("\nYour Email changed");
                    Sleep(2000);
                    break;
                }
                fclose(user_info);
            }
        }
        else if (input == 3) {
            clear();
            FILE* match_info = fopen("matchdata.txt", "r");
            int games = 0;
            int wins = 0;
            int lost = 0;
            printf("HISTORY :\n\n");
            //printf("player1     ")
            while (fread(&mr, sizeof(match_result), 1, match_info)) {
                if (strcmp(ui.user1, mr.player1) == 0) {
                    ++games;
                    printf("1:%s    2:%s", mr.player1, mr.player2);
                    if (mr.game_status == 1) {
                        ++wins;
                        printf("    %s is winner\n", mr.player1);
                    }
                    else if (mr.game_status == 2) {
                        ++lost;
                        printf("    %s is winner\n", mr.player2);
                    }
                    else printf("    Match was tied\n");
                }
                else if (strcmp(ui.user1, mr.player2) == 0) {
                    ++games;
                    printf("1:%s    2:%s", mr.player1, mr.player2);
                    if (mr.game_status == 1) {
                        ++lost;
                        printf("    %s is winner\n", mr.player1);
                    }
                    else if (mr.game_status == 2) {
                        ++wins;
                        printf("    %s is winner\n", mr.player2);
                    }
                    else printf("    Match was tied\n");
                }
            }
            printf("Games= %d   Wins= %d   Lost= %d    Equals= %d\nYour score= %d", games, wins, lost, games - (wins + lost), (wins * 20) + ((games - (wins + lost)) * 10));
            fclose(match_info);
            while (1) {
                printf("\nType 1 to back to Menu:");
                int input = get_digit();
                if (input == 1)break;
            }

        }
        else if (input == 4) {
            login_page();
        }
    }
}
int main() {
    login_page();
    strcpy(ui.user1, si.username);
    game_menu();
}

/*bool ends_with(char* s1, char* s2) {
    size_t s1_len = strlen(s1), s2_len = strlen(s2);
    return s2_len <= s1_len && strcmp(s1 + s1_len - s2_len, s2);
}*/