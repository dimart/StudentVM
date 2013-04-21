/*
 * Project: StudentVM
 * Version: 1.0
 * Author: dimart
 * Date: 10.03.13
 */

#include "structures.h"
#include "IOSystem.h"
#include "memory.h"
#include "stack.h"
#include "exceptionHandler.h"
#include "core.h"

int main(int argc, char* argv[])
{
    /* create VirtualMachine */
    VM SVM;
    SVM.errorCode = 0;
    SVM.mode = 0;

    /* check arguments */
    if (checkArgs(argc, argv, &SVM.mode)) return 1;

    /* check configuration file */
    checkConfig(&SVM.config, &SVM.errorCode);
    if (SVM.errorCode != 0) return 1;

    /* initialization memory */
    SVM.memory.space = createMemory(SVM.config.memorySize);
    if(SVM.memory.space == NULL) return 1;
    SVM.memory.currentAddress = SVM.config.memorySize - 1;

    /* initialization stack */
    SVM.stack = initStack();

    /* start */
    if (!runVM(&SVM, argv[1])) return 1;
    return 0;
}
