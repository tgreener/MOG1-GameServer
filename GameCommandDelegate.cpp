
#include "GameCommandDelegate.h"
#include "Server.h"

void GameCommandDelegate::interpretCommand(char* bytes, int length) {
    if(length <= 1) {
        return;
    }
    
    switch(bytes[1]) {
        default :
            ServiceLocator::getServiceLocator().sendMessageToClient("Unrecognized game command.\n");
            break;
    }
}
