#include <ctype.h>
#include "parser.h"
#include "IOSystem.h"
#include "exceptionHandler.h"
#define isDigit(a) a >= '0' && a <= '9'
#define isLetter(b) b >= 'a' && b <= 'z'

int readCodeLine(FILE **input, char *string)
{
    int k = 0;
    int charCount = SUPPLIER;
    char ch;

    while (((ch = fgetc(*input)) != '\n') && !feof(*input))
    {
        if (k == charCount)
        {
            string = realloc(string, charCount += SUPPLIER);
            if (string == NULL)
            {
                error(MEMORY_ALLOCATION_ERROR);
                return 0;
            }
        }
        string[k++] = tolower(ch);
    }
    string[k] = 0;
    return 1;
}

int parsing(Code* code, const char* fileName)
{
    Expression expr;
    int currentCountOfCodelines = 0;
    int defCountOfCodelines = DEF_COUNT_OF_CODLINE;
    int haveError = 0;

    code->countOfLabels = 0;
    code->commands = (Command*) malloc(sizeof(Command) * defCountOfCodelines);
    expr.line = (char*) malloc(sizeof(char*) * SUPPLIER);
    if (expr.line == NULL || code->commands == NULL)
    {
        error(MEMORY_ALLOCATION_ERROR);
        return 0;
    }
    if (!openFile(&code->file, fileName))
    {
        printf("%s", fileName);
        error(CANNOT_OPEN_CODE_FILE);
        return 0;
    }

    //Start parse
    while (!feof(code->file))
    {
        if (currentCountOfCodelines < defCountOfCodelines)
        {
            if (readCodeLine(&code->file, expr.line))
            {
                if (!parseLine(code, &expr, &currentCountOfCodelines)) haveError = 1;
            }
            else break;
        }
        else if (currentCountOfCodelines >= defCountOfCodelines)
        {
            code->commands = (Command*) realloc(code->commands, sizeof(Command) * currentCountOfCodelines * 2);
            if (code->commands == NULL)
            {
                error(MEMORY_ALLOCATION_ERROR);
                free(expr.line);
                fclose(code->file);
                return 0;
            }
            if (readCodeLine(&code->file, expr.line))
            {
                if(!parseLine(code, &expr, &currentCountOfCodelines)) haveError = 1;
            }
            else break;
        }
        currentCountOfCodelines++;
    }

    if (haveError)
    {
        error(SYNTAX_ERROR);
        free(expr.line);
        fclose(code->file);
        return 0;
    }
    code->currentCountOfLines = currentCountOfCodelines;
    free(expr.line);
    fclose(code->file);
    return 1;
}

int parseLine(Code *code, Expression *expr, int *num)
{
    code->commands[*num] = expression(expr);
    if (code->commands[*num].opcode == ERR)
    {
        fprintf(stderr, "Cannot parse line number %d.\n", (*num)+1);
        return 0;
    }
    if (code->commands[*num].opcode == LBL)
    {
        code->commands[*num].labelLine = *num;
        code->countOfLabels++;
    }
    return 1;
}

Command expression(Expression *expr)
{
    Command result;

    expr->type = NUL;
    expr->current = 0;
    result = next(expr);
    if (result.opcode == ERR || expr->type == comment) // || result.opcode == LBL)
    {
        return result;
    }
    else if (expr->type != onepart)
    {
        Command following = next(expr);
        if (expr->type == twopartAdr)
        {
            if (following.opcode == NUM)
            {
                result.args.address = following.args.address;
            }
        }
        else if (expr->type == twopartNum)
        {
            if (following.opcode == NUM)
            {
                result.args.number = following.args.number;
            }
        }
        else if (expr->type == twopartLab)
        {
            if ((following.opcode != ERR) && (expr->type != comment))
            {
                result.args.label = following.args.label;
            }
        }
        if (following.opcode == ERR)
        {
            result.opcode = ERR;
            return result;
        }
    }
    next(expr);
    if ((expr->type != NUL) && (expr->type != comment))
    {
        result.opcode = ERR;
    }
    return result;
}

Command next(Expression *expr)
{
    Command result;

    char token = expr->line[expr->current];
    if (token == '\0')
    {
        expr->type = NUL;
        return result;
    }
    else if (token == ';')
    {
        expr->type = comment;
        return result;
    }
    else if (token == ' ')
    {
        expr->current++;
        return next(expr);
    }
    else if (isLetter(token) && (expr->type != twopartLab))
    {
        return getCommand(expr);
    }
    else if (expr->type == twopartLab)
    {
        return getLabel(expr);
    }
    else if ((token == '-' || (isDigit(token))) && (expr->type != onepart))
    {
        return getNumber(expr);
    }
    else
    {
       result.opcode = ERR;
    }
    return result;
}

char* concat(char *string, char ch)
{
    int i = 0;
    while(string[i] != '\0') i++;
    string[i] = ch;
    string[++i] = '\0';
    return string;
}

Command getLabel(Expression *expr)
{
    Command result;
    int current = expr->current;
    char token = expr->line[current];
    char* label;
    label = (char*) calloc(sizeof(char), MAX_LENGTH_OF_LABEL);
    if (label == NULL)
    {
        fprintf(stderr, "Too long label name.\n");
        error(MEMORY_ALLOCATION_ERROR);
        result.opcode = ERR;
        return result;
    }

    while ((isLetter(token)) || (isDigit(token)))
    {
        label = concat(label, token);
        token = expr->line[++current];
    }

    result.args.label = label;
    expr->current = current;
    return result;
}

Command getNumber(Expression *expr)
{
    Command number;

    int num = 0;
    int isNegative = 0;
    int current = expr->current;
    char token = expr->line[current];

    if (token == '-')
    {
        isNegative = 1;
        token = expr->line[++current];
    }

    while (isDigit(token))
    {
        num = 10 * num + (token - '0');
        token = expr->line[++current];
    }

    expr->current = current;
    if ((expr->type == twopartAdr) && (!isNegative))
    {
        number.args.address = num;
    }
    else if ((expr->type == twopartNum) && (!isNegative))
    {
        number.args.number = num;
    }
    else if ((expr->type == twopartNum) && isNegative)
    {
        number.args.number = num - 2*num;
    }
    else
    {
        error(OUT_OF_BOUNDS_ADDRESS);
        number.opcode = ERR;
        return number;
    }

    number.opcode = NUM;
    return number;
}

Command getCommand(Expression *expr)
{
    Command command;

    int current = expr->current;
    char token = expr->line[current];
    char* string;
    string = (char*) calloc(sizeof(char), MAX_LENGTH_OF_LABEL);
    if (string == NULL)
    {
        fprintf(stderr, "Too long command name.\n");
        error(MEMORY_ALLOCATION_ERROR);
        command.opcode = ERR;
        return command;
    }

    while ((isLetter(token)) || (isDigit(token)))
    {
        string = concat(string, token);
        token = expr->line[++current];
    }

    /* LABEL DEFINITION */
    if (token == ':')
    {
        expr->current = current + 1;
        expr->type = onepart;
        command.opcode = LBL;
        command.labeldef = string;
        return command;
    }
    else if (strcmp(string, "ldc") == 0)
    {
        command.opcode = LDC;
        expr->type = twopartNum;
    }
    else if (strcmp(string, "ldi") == 0)
    {
        command.opcode = LDI;
        expr->type = onepart;
    }
    else if (strcmp(string, "ld") == 0)
    {
        command.opcode = LD;
        expr->type = twopartAdr;
    }
    else if (strcmp(string, "st") == 0)
    {
        command.opcode = ST;
        expr->type = twopartAdr;
    }
    else if (strcmp(string, "sti") == 0)
    {
        command.opcode = STI;
        expr->type = onepart;
    }
    else if (strcmp(string, "sub") == 0)
    {
        command.opcode = SUB;
        expr->type = onepart;
    }
    else if (strcmp(string, "add") == 0)
    {
        command.opcode = ADD;
        expr->type = onepart;
    }
    else if (strcmp(string, "mul") == 0)
    {
        command.opcode = MUL;
        expr->type = onepart;
    }
    else if (strcmp(string, "mod") == 0)
    {
        command.opcode = MOD;
        expr->type = onepart;
    }
    else if (strcmp(string, "div") == 0)
    {
        command.opcode = DIV;
        expr->type = onepart;
    }
    else if (strcmp(string, "cmp") == 0)
    {
        command.opcode = CMP;
        expr->type = onepart;
    }
    else if (strcmp(string, "jmp") == 0)
    {
        command.opcode = JMP;
        expr->type = twopartLab;
    }
    else if (strcmp(string, "hlt") == 0)
    {
        command.opcode = HLT;
        expr->type = onepart;
    }
    else if (strcmp(string, "br") == 0)
    {
        command.opcode = BR;
        expr->type = twopartLab;
    }
    else
    {
        command.opcode = ERR;
    }

    expr->current = current;
    free(string);
    return command;
}
