#include "pico/stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ts_lcd.h"
#include "limits.h"
#include "button.h"
#include "timer.h"

enum OPERATOR
{
    ADD,
    SUB,
    MUL,
    DIV,
    CLEAR,
    EQUAL,
};

enum ACTIONTYPE
{
    NUMBER,
    OPERATOR,
} actionType = NUMBER;

enum STATE
{
    INITIAL_STATE,
    INPUT_OPERATOR,
    INPUT_OPERAND,
    ERROR_STATE,
    EQUAL_STATE,
};

struct calc
{
    int32_t num1;
    int32_t num2;
    int32_t *active_operand;
    enum OPERATOR operator;
    enum STATE state;
} c;

void swapActiveOperand(struct calc *c)
{
    if (c->active_operand == &c->num1)
    {
        c->active_operand = &c->num2;
    }
    else
    {
        c->active_operand = &c->num1;
    }
}

void clearCalc(struct calc *c)
{
    c->state = INITIAL_STATE;
    c->num1 = 0;
    c->num2 = 0;
    c->active_operand = &c->num1;
    c->operator= EQUAL;
}

void addToOperand(int32_t *operand, int value)
{
    int result = *operand * 10 + value;
    if (((*operand > 0) && (result < *operand)) || result > INT32_MAX)
    {
        return; // dont do anything because we are at the max number so just ignore the input
    }
    *operand = result; // set the value
}

int btnCnt = 0;

uint32_t current_time, button_debounce_time;

struct Button buttons[16];

void initButton(struct Button b)
{
    buttons[btnCnt] = b;
    btnCnt++;
}
void main()
{

    clearCalc(&c);

    initButton(createButton(0, 40, 83, 50, '7'));
    initButton(createButton(83, 40, 83, 50, '8'));
    initButton(createButton(83 * 2, 40, 83, 50, '9'));
    initButton(createButton(0, 50 * 1 + 40, 83, 50, '4'));
    initButton(createButton(83, 50 * 1 + 40, 83, 50, '5'));
    initButton(createButton(83 * 2, 50 * 1 + 40, 83, 50, '6'));

    initButton(createButton(0, 50 * 2 + 40, 83, 50, '1'));
    initButton(createButton(83, 50 * 2 + 40, 83, 50, '2'));
    initButton(createButton(83 * 2, 50 * 2 + 40, 83, 50, '3'));
    initButton(createButton(0, 50 * 3 + 40, 83, 50, '0'));
    // the next set of buttons
    initButton(createButton(83 * 3, 50 * 0 + 40, 83, 50, '+'));
    initButton(createButton(83 * 3, 50 * 1 + 40, 83, 50, '-'));
    initButton(createButton(83 * 3, 50 * 2 + 40, 83, 50, '/'));
    initButton(createButton(83 * 3, 50 * 3 + 40, 83, 50, 'x'));
    initButton(createButton(83 * 2, 50 * 3 + 40, 83, 50, 'C'));
    initButton(createButton(83 * 1, 50 * 3 + 40, 83, 50, '='));

    ts_lcd_init();

    current_time = timer_read();
    for (int i = 0; i < btnCnt; i++)
    {
        buttons[i].time = current_time;
    }

    while (1)
    {
        current_time = timer_read();
        uint16_t x, y;
        bool isTouched = get_ts_lcd(&x, &y);
        for (int i = 0; i < btnCnt; i++)
        {
            if (timer_elapsed_ms(buttons[i].time, current_time) >= BUTTON_PRESSED_MS)
            {
                tick_button(&buttons[i], x, y, isTouched);
                button_debounce_time = current_time;
            }
        }

        for (int i = 0; i < btnCnt; i++)
        {
            if (!buttons[i].pressed)
            {
                continue;
            }

            int value = buttons[i].c & 0b00001111;
            if (value >= 0 && value <= 9)
            {
                actionType = NUMBER;
            }
            else
            {
                actionType = OPERATOR;
            }

            // assign the operator based on the char of the button pressed
            enum OPERATOR operator= CLEAR;
            if (actionType == OPERATOR)
            {
                switch (buttons[i].c)
                {
                case '+':
                    operator= ADD;
                    break;
                case '-':
                    operator= SUB;
                    break;
                case '/':
                    operator= DIV;
                    break;
                case 'x':
                    operator= MUL;
                    break;
                case '=':
                    operator= EQUAL;
                    break;
                case 'C':
                case 'c':
                    operator= CLEAR;
                    break;
                default:
                    break;
                }
            }

            if (actionType == OPERATOR && operator== CLEAR)
            {
                clearCalc(&c);
                break;
            }

            switch (c.state)
            {
            case EQUAL:
                clearCalc(&c);
                // continue to initial state

            case INITIAL_STATE:
                if (actionType == NUMBER)
                {
                    addToOperand(c.active_operand, value);
                }
                else if (actionType == OPERATOR)
                {
                    if (operator== EQUAL)

                        c.state = INPUT_OPERATOR;
                    c.operator= operator;
                    swapActiveOperand(c);
                }
                break;
            case INPUT_OPERATOR:
                if (actionType == OPERATOR)
                {
                    c.operator= operator;
                }
                else if (actionType == NUMBER)
                {
                    addToOperand(c.active_operand, value);
                    c.state = INPUT_OPERAND;
                }
                break;
            case INPUT_OPERAND:
                if (actionType == NUMBER)
                {
                    addToOperand(c.active_operand, value);
                }
                else if (actionType == OPERATOR)
                {

                    // run the operation in c.operator on the numbers and store in c.num1
                    switch (c.operator)
                    {
                    case ADD:
                        c.num1 = c.num1 + c.num2;
                        break;
                    case SUB:
                        c.num1 = c.num1 - c.num2;
                        break;
                    case MUL:
                        c.num1 = c.num1 * c.num2;
                        break;
                    case DIV:
                        if (c.num2 != 0)
                        {
                            c.num1 = c.num1 / c.num2;
                        }
                        else
                        {
                            c.state = ERROR_STATE;
                        }
                        break;
                    default:
                        break;
                    }

                    if (operator== EQUAL)
                    {
                        c.state = EQUAL_STATE;
                    }
                    else
                    {
                        c.operator= operator;
                        c.state = INPUT_OPERATOR;
                        swapActiveOperand(&c);
                    }
                }
                break;
            }
        }

        for (int i = 0; i < btnCnt; i++)
        {
            render_button(&buttons[i]);
        }

        // display based on state
        // INITIAL_STATE,
        // INPUT_OPERATOR,
        // INPUT_OPERAND,
        // ERROR_STATE,
        // EQUAL_STATE,
        switch (c.state)
        {
        case EQUAL_STATE:
        case INITIAL_STATE:
            render_text_number(&c.num1);
            break;
        case ERROR_STATE:
            render_text("Error");
            break;
        case INPUT_OPERAND:
            render_text_number(c.active_operand);
            break;

        case INPUT_OPERATOR:
            switch (c.operator)
            {
            case ADD:
                render_text("+");
                break;
            case SUB:
                render_text("-");
                break;
            case MUL:
                render_text("*");
                break;
            case DIV:
                render_text("/");
                break;
            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}
