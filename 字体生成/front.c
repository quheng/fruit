#include <stdio.h>
#include <stdlib.h>
#include "picture.c" 

int main(void)
{
    FILE* fphzk = NULL;
    int i, j, k, offset,num=0,t,tem,tt;
    int flag;
    unsigned char buffer[32];
    unsigned char word[5];
    unsigned char key[8] = {
        0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01
    };
    int output[64][400];
    memset(output,0,sizeof(output));
    fphzk = fopen("hzk16", "rb");
    if(fphzk == NULL){
        fprintf(stderr, "error hzk16\n");
        return 1;
    }
        printf("输入要生成字模的汉字(多个):");
        for(;;){
            fgets((char*)word, 3, stdin);
            if(*word == '\n') 
                break;
            offset = (94*(unsigned int)(word[0]-0xa0-1)+(word[1]-0xa0-1))*32;
            fseek(fphzk, offset, SEEK_SET);
            fread(buffer, 1, 32, fphzk);
            for(k=0; k<16; k++){
                for(j=0; j<2; j++){
                    for(i=0; i<8; i++){
                        flag = buffer[k*2+j]&key[i];
                        printf("%s", flag?"●":"○");
                        for (t=0;t<4;t++)
							for (tt=0;tt<4;tt++)
                        		if (flag!=0) output[k*4+t][(j*8+i)*4+tt+num*64]=255;
                        		else output[k*4+t][(j*8+i)*4+tt+num*64]=0;
                    }
                }
                printf("\n");
            }
            printf("uchar code key[32] = {");
            for(k=0; k<31; k++){
                printf("0x%02X,", buffer[k]);
            }
            printf("0x%02X};\n", buffer[31]);
            printf("\n");
            num++;
        }
    printf("%d ",num*64);
    fclose(fphzk);
    fphzk = NULL;
    fphzk=fopen("front.txt", "w");
    printf("\n");
	for (i=0;i<64;i++){
    		for (j=0;j<64*num;j++)
    			fprintf(fphzk,"%d,",output[i][j]);
   		 } 
   	getchar(); 
    return 0;
}
