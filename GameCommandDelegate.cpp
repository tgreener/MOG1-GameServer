
#include "GameCommandDelegate.h"
#include "ServiceLocator.h"

void GameCommandDelegate::interpretCommand(char* bytes, int length) {
    if(length > 1) {
        int index = bytes[1];
        if(index < functions.size() && index > 0) functions[index](bytes, length);
        else functions[0](nullptr, 0);
    }
}

GameCommandDelegate::GameCommandDelegate() {
    functions.push_back([](const char* bytes, int length) -> void {
        ServiceLocator::getServiceLocator().sendMessageToClient("Unrecognized game command.");
    });
    
    
}
