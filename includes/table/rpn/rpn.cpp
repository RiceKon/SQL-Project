#include "../table.h"

vector<long> RPN::eval(){
    Stack<Token *> holder;
    Queue<Token *> temp = _post_fix;
    Token *left, *right, *result;

    while(!temp.empty()){
        Token *current = temp.pop();
        switch (current->get_type())
        {
        case TOKEN_STR:
            holder.push(current);
            break;
        case RELATIONAL:
        case LOGICAL:
        {
            // post condition: the stack should have a 2 result sets
            right = holder.pop();   
            left = holder.pop();
            if(current->get_type() == RELATIONAL){
                result = static_cast<Relational *>(current)->eval(left, right, t);
            }else
                result = static_cast<Logical *>(current)->eval(left, right); 

            holder.push(result);
        }
            break;
        default:
            break;
        }
    }

    return static_cast<ResultSet *> (holder.pop())->get_result();
}

void RPN::set_post_fix(Queue<Token*> post_fix){
    _post_fix = post_fix;
}
