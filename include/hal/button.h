#ifndef _BUTTON_H_
#define _BUTTON_H_



namespace SmartSecurity_Esp{
    class Button {
        private:
            int _pin;
        public:
            Button(int pin);
            bool isPressed();
            bool button_state(void* func,void* output);
            
    };
}

#endif // _BUTTON_H_