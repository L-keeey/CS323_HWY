#include "syntax.tab.c"
int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    char output_dir[100];
    for (int i = 0; i < 100; i ++) {
        output_dir[i] = '\0';
    }
    strncpy(output_dir, argv[1], strlen(argv[1])-4);
    strcat(output_dir,".out");
    freopen(output_dir,"w",stdout);
    yyparse();

    if(error_num == 0){
        // pre_order_traversal(root, 0);
    }
    fclose(stdout);
    return 0;
}