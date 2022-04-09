#include <ostream>

extern "C" {
#include <incbin.h>
}

/*
 * Load the templates into the binary because I don't feel like loading them from
 * the filesystem
 */
INCTXT(commandTemplateCppFile, "../templates/command/templateCommand.cpp");
INCTXT(commandTemplateHFile,   "../templates/command/templateCommand.h");

int main() {
    return 0;
}
