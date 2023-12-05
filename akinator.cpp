#include "akinator.h"

int main(){

    FILE* logfile = fopen("logfile.txt", "w");
    FILE* dotfile = fopen("my_dotcode.dot", "w");
    Root root = {};
    RootCtor(&root, logfile);

    printf("root.size = %zu\n", root.size);
    printf("data: %s\n", root.tree_root->data);


    FILE* databasefile = fopen("akinator_database.txt", "r");
    if(databasefile == NULL){
        printf("Can't open database file:(\n");
        return -1;
    }

    char mode_ans[DEFAULT_SIZE] = {};

    printf("Выберите один из трёх режимов работы программы: \"Akinator\"[0],"
    ", \"Definition\"[1], \"Comparation\"[2]\n");
    scanf("%[^\n]", mode_ans);
    printf("mode_ans:%s\n", mode_ans);


    int read_output = ReadTree(databasefile, root.tree_root, logfile);
    if(read_output){
        printf("Bad tree reading.\nExiting...\n");
        return -1;
    }
    fclose(databasefile);

    IpAssignment(&root, 0, logfile);

    printf("mode_ans: %s\n", mode_ans);

    switch(mode_ans[0] - '0'){
        case 0:
            AkinatorGuessing(&root, root.tree_root, logfile);
            break;
        case 1:
            printf("Определение кому из персонажей Вы хотели бы дать?\n");
            while(!getchar());
            scanf("%[^\n]", mode_ans);
            while(!getchar());
            AkinatorDefinition(&root, root.tree_root, mode_ans, logfile);
            break;
        case 2:
            break;
        default:
            printf("Выбран некорректный режим.\n");
            return 0;
    }

    OpGraphDump(&root, dotfile, logfile);

    fclose(logfile);
    fclose(dotfile);
    return 0;
}


/*
line  4
line 31
line 39

*/
