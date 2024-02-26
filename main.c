#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define inc1 10
#define inc2 50

int scan_sentence(char **str) {
    int str_size = inc2;
    int i = 0;
    char symb;
    *str = malloc(str_size*sizeof(char));
    while((symb=getchar())!='.') {
        if(symb==EOF) {
            free(*str);
            return -1;
        } 
        if(i==0 && symb==' ') {
            continue;
        }
        (*str)[i] = symb;
        if(i==str_size-1) {
            str_size+=inc2;
            (*str) = realloc((*str),str_size*sizeof(char));
        }
        i++;
    }
    if(str_size-i<=1) {
        str_size+=inc2;
        (*str) = realloc((*str),str_size*sizeof(char));
    }
    (*str)[i] = '.';
    (*str)[i+1] = '\0';
    return 1;
}

int scan_text(char ***text) {
    int text_size = inc1;
    int num = 0;
    *text = malloc(text_size*sizeof(char*));
    while(scan_sentence(*text+num)!=-1) {
        if(num == text_size-1) {
            text_size+=inc1;
            (*text) = realloc((*text),text_size*sizeof(char*));
        }
        num++;
    }
    return num;
}

void del_null_sentences(char ***text, int *text_size) {
    for(int i = 0; i < *text_size; i++) {
        if((*text)[i]==NULL) {
            memmove((*text+i),(*text+i+1),((*text_size-i)*sizeof(char*)));
            i--;
            (*text_size)--;
        }
    }
}

void delete_equal_sentences(char ***text, int *text_size) {
    char* snt1;
    char* snt2;
    for(int i = 0; i < *text_size; i++) {
        for(int j = i+1; j < *text_size; j++) {
            snt1 = (*text)[i];
            snt2 = (*text)[j];
            if(snt1!=NULL && snt2!=NULL) {
                if(strcasecmp(snt1,snt2)==0) {
                    free((*text)[j]);
                    (*text)[j] = NULL;
                }
            }
        }
    }
    del_null_sentences(text,text_size);
}

void print_text(char **text, int text_size) {
    for(int i = 0; i < text_size; i++) {
        printf("%s",text[i]);
    }
}

void free_text(char **text, int text_size) {
    for(int i = 0; i < text_size; i++) {
        free(text[i]);
    }
    free(text);
}

void print_words(char** text, int text_size) {
    int words_size = inc1;
    char **words = malloc(words_size*sizeof(char*));
    int cur_word = 0;
    char* snt;
    char *istr;
    int flag;
    int count;
    char* word;
    char sep[2] = " ,";
    for(int i = 0; i < text_size; i++) {
        snt = malloc((strlen(text[i])+1)* sizeof(char));
        strcpy(snt,text[i]);
        istr = strtok(snt,sep);
        while(istr!=NULL) {
            if(cur_word == words_size-1) {
                words_size+=inc1;
                words = realloc(words,words_size*sizeof(char*));
            }
            words[cur_word] = istr;
            if(words[cur_word][strlen(words[cur_word])-1]=='.') {
                words[cur_word][strlen(words[cur_word])-1] = '\0';
            }
            cur_word++;
            istr = strtok(NULL,sep);
        }
    }
    for(int i = 0; i < cur_word; i++) {
        count = 0;
        word = words[i];
        if(word!=NULL) {
            count++;
            for(int j = i+1; j < cur_word; j++) {
                if(words[j]!=NULL) {
                    if(strcmp(word,words[j])==0) {
                        count++;
                        words[j] = NULL;
                    }
                }    
            }
            printf("%s : %d\n",word,count);
        }
    }
}

void reverse_every_word(char*** text, int text_size) {
    char** new_text = malloc(text_size*sizeof(char*));
    for(int i = 0; i < text_size; i++) {
        new_text[i] = malloc(strlen((*text)[i])*sizeof(char));
    }
    char* snt;
    char* snt1;
    char* istr;
    char* new_snt;
    char sep[2] = " ,";
    int f;
    int index;
    int stlen;
    for(int i = 0; i < text_size; i++) {
        index = 0;
        snt = malloc((strlen((*text)[i])+1)* sizeof(char));
        snt1 = malloc((strlen((*text)[i])+1)* sizeof(char));
        strcpy(snt,(*text)[i]);
        istr = strtok(snt,sep);
        while(istr!=NULL) {
            f = 0;
            if(strstr(istr,".")!=NULL) {
                f=1;
            }
            if(f==0) {
                stlen = strlen(istr);
                for(int j = 0; j < stlen; j++) {
                    snt1[index] = istr[stlen-j-1];
                    index++;
                }
                snt1[index] = (*text)[i][index];
                index++;
            } else {
                stlen = strlen(istr)-1;
                for(int j = 0; j < stlen; j++) {
                    snt1[index] = istr[stlen-j-1];
                    index++;
                }
                snt1[index] = '.';
                index++;
            }
            istr = strtok(NULL,sep);
        }
        new_text[i] = snt1;
    }
    (*text) = new_text;
}

void delete_comma_sentences(char ***text, int* text_size) {
    int f;
    for(int i = 0; i < *text_size; i++) {
        f = 0;
        for(int j = 0; j < strlen((*text)[i]); j++) {
            if((*text)[i][j]==',') {
                f = 1;
            }
        }
        if(f==1) {
            free((*text)[i]);
            (*text)[i] = NULL;
        }
    }
    del_null_sentences(text,text_size);
}

int is_sep(char sym) {
    if(sym==',' || sym==' ') {
        return 1;
    }
    return 0;
}

int cmp(const void *a, const void *b) {
    const int **a1 = (const int **)a;
    const int **b1 = (const int **)b;
    const int a2 = (*a1)[0];
    const int b2 = (*b1)[0];
    if(a2<b2) {
        return 1;
    }
    if(a2>b2) {
        return -1;
    }
    return 0;
}

void sort_by_5_symb(char ***text, int text_size) {
    char** new_text = malloc(text_size*sizeof(char*));
    int count;
    int len;
    char sym;
    int** sort = malloc(text_size*sizeof(int*));
    for(int i = 0; i < text_size; i++) {
        sort[i] = malloc(2*sizeof(int));
    }
    for(int i = 0; i < text_size; i++) {
        len = strlen((*text)[i]);
        if(len<5) {
            count = -1;
            sort[i][0] = count;
            sort[i][1] = i;
        } else {
            for(int j = 0; j < len; j++) {
                if(j==4) {
                    if(is_sep((*text)[i][j])==1) {
                        sym = (*text)[i][j+1];
                    } else {
                        sym = (*text)[i][j];
                    }
                    count = sym - '\0';
                    sort[i][0] = count;
                    sort[i][1] = i;
                }
            }
        }
    }

    qsort(sort,text_size,sizeof(int*),cmp);

    for(int i = 0; i < text_size; i++) {
        new_text[i] = (*text)[sort[i][1]];
    }

    (*text) = new_text;

}

int main()
{

    int text_size, str_size;
    char **text;

    printf("\nЧто вы хотите сделать с текстом?\n1)Распечатать каждое слово и количество его повторений в тексте.\n2)Преобразовать каждое предложение так, что символы в каждом слове шли в обратном порядке.\n3)Удалить предложения в котором встречается запятая.\n4)Отсортировать предложения по уменьшению значения кода 5 символа предложения.\n5)Удалить повторяющиеся предложения и выйти из программы.\n");
    int menu;
    scanf("%d",&menu);
    char symb = getchar();

    printf("Введите текст:\n");
    text_size = scan_text(&text);
    printf("Повторяющиеся предложения удалены:\n");
    delete_equal_sentences(&text,&text_size);
    print_text(text,text_size);
    printf("\n");
    
    
    switch(menu) {
        case 1:
            print_words(text,text_size);
            break;
        case 2:
            printf("Новый текст:\n");
            reverse_every_word(&text,text_size);
            print_text(text,text_size);
            break;
        case 3:
            printf("Новый текст:\n");
            delete_comma_sentences(&text,&text_size);
            print_text(text,text_size);
            break;
        case 4:
            printf("Новый текст:\n");
            sort_by_5_symb(&text,text_size);
            print_text(text,text_size);
            break;
        case 5:
            break;
        default:
            printf("\nТакой функции нет.\n");
    }

    free_text(text,text_size);

    return 0;
}
