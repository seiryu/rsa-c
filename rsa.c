#include <stdio.h>
#include <stdlib.h>

//  y ≡ x^a( mod b) を計算する関数
int cal(int x, int a, int b){ 
    int i,y;
    y = 1;
    for(i=0; i<a; i++){
        y = y*x % b;
    }
    return y;
}

// 暗号化の関数
// 引数は (e,n,[平文ファイル名],[暗号ファイル名])
void encrypt(int e, int n, char *fnr, char *fnw){
    FILE *ifp, *ofp;
    int m,c,c0,c1;
    if((ifp = fopen(fnr, "r")) == NULL){  // 平文ファイルを"r"でオープン
        perror(fnr);
        exit(1); // エラーが発生した場合は終了
    }
    if((ofp = fopen(fnw, "w")) == NULL){  // 暗号化ファイルを"w"でオープン
        perror(fnr);
        exit(1); // エラーが発生場合は終了
    }
    // 1バイトずつ取り出し、暗号化して2バイトで出力
    while( (m = fgetc(ifp)) != EOF){
        c = cal(m,e,n);
        c0 = c % 256;
        c1 = (c-c0)/256;
        fputc(c0,ofp);
        fputc(c1,ofp);
    }
    fclose(ifp);
    fclose(ofp);
    return;
}

// 復号化の関数
// 引数は (d,n,[暗号ファイル名],[復号ファイル名])
void decrypt(int d, int n, char *fnr, char *fnw){
    FILE *ifp, *ofp;
    int m, c,c0, c1;

    if((ifp = fopen(fnr, "r")) == NULL){  // 平文ファイルを"r"でオープン
        perror(fnr);
        exit(1); // エラーが発生した場合は終了
    }
    if((ofp = fopen(fnw, "w")) == NULL){  // 暗号化ファイルを"w"でオープン
        perror(fnr);
        exit(1); // エラーが発生場合は終了
    }
    // 2バイトずつ取り出し、復号化して1バイトで出力
    while( (c0 = fgetc(ifp)) != EOF && (c1 = fgetc(ifp)) != EOF){
        c = c0+256*c1;
        m = cal(c,d,n);
        fputc(m,ofp);
    }
    fclose(ifp);
    fclose(ofp);
    return;
}

int main(int argc, char *argv[]){
    char err_message[] = "usage : rsa -[ed] e/d n InputFILE OutputFILE\n";
    if(argc == 6){
        switch(argv[1][1]){
        case 'e': // 暗号化の場合
            encrypt(atoi(argv[2]), atoi(argv[3]), argv[4], argv[5]);
            break;
        case 'd': // 復号化の場合
            decrypt(atoi(argv[2]), atoi(argv[3]), argv[4], argv[5]);
            break;
        default:        
            printf("%s",err_message);
            break;
        }
    }else{
        printf("%s",err_message);
    }
    return 0;
}
