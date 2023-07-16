#include <include/pre_processor.h>
#include <include/helper_fucntions.h>
int main(int argc, char const *argv[])
{
    FILE *tar_file;
    int arg_index;
    for ( arg_index = 1; arg_index < argc; arg_index++)
    {
        tar_file = create_file(argv[arg_index], ".am");
        process_file(fopen(argv[arg_index], "r"), tar_file);
    }
    
    return 0;
}
