#include "tac.h"
#include "mips32.h"

#define BUF_SIZE 0x10000

char buf[BUF_SIZE];

int main(int argc, char *argv[]){
    FILE *fp;
    tac *head;
    char c, *file;
    int size, len;
    debug=fopen("debug_log","w");
    if(argc != 2){
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <IR-file>\n", argv[0]);
        return 1;
    }
    file = argv[1];

    // read the IR code
    size = 0;
    sout("reading IR\n");
    fp = fopen(file, "r");
    while ((c = getc(fp)) != EOF){
        buf[size++] = c;
    }
    buf[size] = '\x7f';
    fclose(fp);
    sout("reading finish\n");

    // write the target code
    len = strlen(file);
    file[len-2] = 's';
    file[len-1] = '\0';
    //fp = stdout; // 

    char output_dir[100];
    for (int i = 0; i < 100; i ++) {
        output_dir[i] = '\0';
    }
    strncpy(output_dir, argv[1], strlen(argv[1])-2);
    strcat(output_dir,".s");
    fopen(output_dir,"w");

    // fopen("mips.s", "w");
    sout("generating code\n");
    head = tac_from_buffer(buf);
    sout("loading IR finish\n");
    mips32_gen(head, fp);
    sout("generation finish\n");
    fclose(fp);

    return 0;
}
