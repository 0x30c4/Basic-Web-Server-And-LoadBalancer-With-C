#include <common.h>
#include <server.h>


void load_conf(const char *conf_file, Conf *conf, Backend bc[], int *bc_len);

int main(int argc, char *argv[]){

    char *server_conf = malloc(BUFSIZE);

    if (argc != 2)
        memcpy(server_conf, "./server.conf\0", 14);
    else
        memcpy(server_conf, argv[1], strlen(argv[1]));

    Conf conf;
    Backend bc[10];
    int backend_len = 0;

    load_conf(server_conf, &conf, bc, &backend_len);

    printf("Server Config : %s\n", server_conf);
    printf("Server Mime: %s\n", conf.mime_file);
    printf("Listen Port: %d\n", conf.listen_port);
    printf("Static Media Root : %s\n", conf.www_path);
    printf("Reverse Proxy Backends: \n");

    for (int i = 0; i < backend_len; i++)
        printf("\tBackend No: %d | %s:%d\n", i + 1, bc[i].address, bc[i].port);

    if (backend_len)
        epoll_driver(conf.listen_port, bc, backend_len, ROUND_ROBIN, conf.mime_file);
    else {

        if (chdir(conf.www_path) != 0) {
            perror("Can't change directory to");
            exit(EXIT_FAILURE);
        }

        epoll_driver(conf.listen_port, bc, backend_len, STATIC_SERVER, conf.mime_file);
    }


    // malloc alocation.
    for (int i = 0; i < backend_len; i++)
        free(bc[i].address);
    free(conf.www_path);
    free(conf.mime_file);
	return 0;
}

/*
 * Function: load_conf
 * ----------------------------
 *   Creates a reverse proxy for the backend. 
 *   
 *   conf_file: the conf file to load data from.
 *   conf: the parsed Conf file.
 *
 *   returns: void
 */

void load_conf(const char *conf_file, Conf *conf, Backend bc[], int *bc_len) {

    FILE *fp = fopen(conf_file, "r");

    if (!fp) {
        fprintf(stderr, "Can't open %s file.", conf_file);
        exit(EXIT_FAILURE);
    }

    char *line;
    size_t v, ve, linesize = 0;

    while (!feof(fp)) {

        getdelim(&line, &linesize, ';', fp);

        if (strstr(line, "www_path")) {
            v = strcspn(line, ":");
            line += v + 1;
            while (line[0] == ' ')
                line++;
            ve = strcspn(line, ";\n");
            conf->www_path = malloc(ve + 1);
            memcpy(conf->www_path, line, ve);
            conf->www_path[ve + 1] = '\0';
            /* printf("%s\n", conf->www_path); */
        }
        if (strstr(line, "listen_port")) {
            v = strcspn(line, ":");
            line += v + 1;
            while (line[0] == ' ')
                line++;
            ve = strcspn(line, ";\n");
            conf->listen_port = atoi(line);
            /* printf("%d\n", conf->listen_port); */
        }
        if (strstr(line, "mime_file")) {
            v = strcspn(line, ":");
            line += v + 1;
            while (line[0] == ' ')
                line++;
            ve = strcspn(line, ";\n");
            conf->mime_file = malloc(ve + 1);
            memcpy(conf->mime_file, line, ve);
            conf->mime_file[ve + 1] = '\0';
            /* printf("%s\n", conf->mime_file); */
        }
        if (strstr(line, "backends_")) {
            v = strcspn(line, ":");
            line += v + 1;
            while (line[0] == ' ')
                line++;
            ve = strcspn(line, ";\n");
            line[ve+1] = '\0';
            line[ve] = '\0';

            ve = strcspn(line, ":");

            bc[*bc_len].address = malloc(ve);

            memcpy(bc[*bc_len].address, line, ve);

            line += ve + 1;

            bc[*bc_len].port = atoi(line);

            /* printf("%s:%d\n", bc[*bc_len].address, bc[*bc_len].port); */
            (*bc_len)++;
        }
    }
    fclose(fp);

}
