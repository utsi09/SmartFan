#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include <wiringSerial.h>
#include<stdlib.h>
#define BAUD_RATE 115200 //블루투스의 보율이 바뀔 경우 이 값을 변경해야함
#define Stop 16
#define PWM0 18
#define PWM1 19
#define BUTTON 21
#define SP1 33
#define SP2 66
#define SP3 100
#define trigPin 14
#define echoPin 15   
#define sound 20
#define LED1 22
#define LED2 23
#define LED3 24

pthread_mutex_t lock;


int power = 0;
int speed = 0, dir=1;
int w_dir = 0;
int s_dir = 0;
int curAngle = 0;

int pin_arr[4] = {8, 9, 10, 11};
int one_phase[8][4] = {
   {1,0,0,0},
   {1,1,0,0},
   {0,1,0,0},
   {0,1,1,0},
   {0,0,1,0},
   {0,0,1,1},
   {0,0,0,1},
   {1,0,0,1}};

static const char* UART2_DEV = "/dev/ttyAMA1";
unsigned char serialRead(const int fd);
void serialWrite(const int fd, const unsigned char c);

unsigned char serialRead(const int fd)
{
   unsigned char x;
   if(read (fd, &x, 1) != 1)
      return -1;
   return x;
}

void serialWrite(const int fd, const unsigned char c)
{
   write (fd, &c, 1);
}
   
void one_two_Phase_Rotate_Angle(float angle, int dir)
{
   int steps = angle * 11.3777;
   int temp = angle;
   //int steps = angle * 9;
   switch(dir){
      case 0:
      for(int i = steps; i>=0; i--){
         
         curAngle = i / 11.3777;
         curAngle += 1;
      
         if (power == 0 || w_dir == 0)
         {
         
         
            FILE* fp = fopen("setting.txt","w");
   
            if(fp != NULL)
            {
               char buf1[10];
               sprintf(buf1,"%d\n", curAngle);
               fputs(buf1,fp);
               
               char buf2[10];
               sprintf(buf2,"%d", s_dir);
               fputs(buf2,fp);
               fclose(fp);
            }

             break;
         }
         digitalWrite(pin_arr[0], one_phase[i%8][0]);
         digitalWrite(pin_arr[1], one_phase[i%8][1]);
         digitalWrite(pin_arr[2], one_phase[i%8][2]);
         digitalWrite(pin_arr[3], one_phase[i%8][3]);
         delay(2);
         
         }
         break;
      case 1:         
      for(int i = 0; i<steps; i++){
         
               
         curAngle = i / 11.3777;
         curAngle = temp - curAngle -1;
         
      
         if (power == 0 || w_dir == 0)
         {
         
         
            FILE* fp = fopen("setting.txt","w");
   
            if(fp != NULL)
            {
               char buf1[10];
               sprintf(buf1,"%d\n", curAngle);
               fputs(buf1,fp);
               
               char buf2[10];
               sprintf(buf2,"%d", s_dir);
               fputs(buf2,fp);
               fclose(fp);
            }

             break;
         }
         digitalWrite(pin_arr[0], one_phase[i%8][0]);
         digitalWrite(pin_arr[1], one_phase[i%8][1]);
         digitalWrite(pin_arr[2], one_phase[i%8][2]);
         digitalWrite(pin_arr[3], one_phase[i%8][3]);
         delay(2);
      }
         
   }

}
void init_Step()
{
//4
   for(int i = 0 ; i< 4; i++)
{
   pinMode(pin_arr[i], OUTPUT);
}
}

void motor_rotate(int speed, int dir)
{

   switch(dir)
   {
      case 0:
         if(speed != 0) {
            pwmWrite(PWM0, speed);
            pwmWrite(PWM1, 0);
         }
         break;
      case 1:
         pwmWrite(PWM1, speed);
         pwmWrite(PWM0, 0);
         break;
   
   }

}

void init()
{
   pinMode (trigPin, OUTPUT);
   pinMode (echoPin, INPUT);
   pinMode (sound, OUTPUT);
   pinMode(BUTTON,INPUT);
   pinMode(LED1, OUTPUT);
   pinMode(LED2, OUTPUT);
   pinMode(LED3, OUTPUT);
   pinMode(PWM0, PWM_OUTPUT);  
   pinMode(PWM1, PWM_OUTPUT);
   pwmSetMode(PWM_MODE_MS);
   
   int length;
   FILE* fp = fopen("setting.txt","r");
   if (fp != NULL)
   {
      char buf1[10];
      fgets(buf1, 10, fp);
      curAngle = atoi(buf1);
      
      
      char buf2[10];
      fgets(buf2, 10, fp);
      s_dir = atoi(buf2);
      
      fclose(fp);
   }   
   printf("a: %d,    w: %d\n",curAngle,s_dir);
}


void *motor(){

   int range = 101;
   int divisor;
   int temp = 0;
   divisor = 19200 / range;
   
   pwmSetRange(range);
   pwmSetClock(divisor);
   pwmWrite(PWM1, 0);
   pwmWrite(PWM0, 0);

   while(1)
   {
      if(digitalRead(BUTTON)==0){
         dir = 1;

         
         pthread_mutex_lock(&lock);
         if(power ==1){
            power = 0;
            speed = 0;         
         }
         else if(power ==0){
         power = 1;
         speed = SP1;
            
         }
         pthread_mutex_unlock(&lock);
         delay(500);
         if(temp <= 0) {
            temp = 1;
         }
         else if(temp >= 1) {
            temp = 0;
         }
      }
      
      if(power ==1) {
         motor_rotate(speed , dir);
         if(temp == 1){
         printf("전원 ON!\n");
         }
         temp= temp + 1;
      }
      else {
         motor_rotate(0 , dir);
         if(temp == 0){
         printf("전원 OFF!\n");
         }
         temp= temp  - 1;
      }

      delay(10);
   }
}

void *ultra() {

   int distance=0;
     int pulse = 0;
   
     long startTime;
     long travelTime;
   int tempSpeed;
   int tempPower;
   int temp= 0;
   while(1)
   {
      
      digitalWrite (trigPin, LOW);
      delay(1); //5hz
      digitalWrite (trigPin, HIGH);
      delay(2);
      digitalWrite (trigPin, LOW);
      

      while(digitalRead(echoPin) == 0);
         startTime = micros();
      
      while(digitalRead(echoPin) == 1);
         travelTime = micros() - startTime;
      
      int distance = travelTime / 58;
      
      //printf( "Distance: %dmm\n", distance * 10);
      
      if(distance >20 && distance < 30){
         digitalWrite(sound, HIGH);
         delay(200);
         digitalWrite(sound, LOW);
         delay(200);
         digitalWrite(sound, HIGH);
         delay(200);
         digitalWrite(sound, LOW);
         delay(200);
      }
      else if(distance < 20) {
         if(temp == 0 ){
            tempSpeed = speed;
            tempPower = power;
            temp = 1;
         }
         dir = 0;
         if(tempSpeed == SP1){
            delay(250);
         }
         else if(tempSpeed == SP2){
            delay(230);
         }
         else if(tempSpeed == SP3){
            delay(150);
         }
         dir = 1;
         speed = 0;
         power = 0;
         digitalWrite(sound, HIGH);
         delay(50);
         digitalWrite(sound, LOW);
         delay(50);
         digitalWrite(sound, HIGH);
         delay(50);
         digitalWrite(sound, LOW);
         delay(50);


      }
      else{
         if(temp == 1) {
            power = tempPower;
            speed = tempSpeed;
            temp = 0;
         }
         digitalWrite(sound, LOW);
      }
      delay(200);
   }
   
}

void *serial(){

   int fd_serial ; //디바이스 파일 서술자
   unsigned char dat; //임시 데이터 저장 변수
   
   if ((fd_serial = serialOpen (UART2_DEV, BAUD_RATE)) < 0) //UART2 포트 오픈
   {   
      printf ("Unable to open serial device.\n") ;
      return 0;
   }
   
   while(1){
      if(serialDataAvail (fd_serial) ){ //읽을 데이터가 존재한다면,
         dat = serialRead (fd_serial); //버퍼에서 1바이트 값을 읽음
         printf ("%c", dat) ;
         
         
         pthread_mutex_lock(&lock);
         switch(dat)
         {
            case '0':
               if(power ==1){
                  power = 0;
                  speed = 0;
                  w_dir = 0;         
               }
               else if(power ==0){
               power = 1;
               speed = SP1;
               }
               break;
            case '1':
               power = 1;
               speed = SP1;
               printf("단");
               break;
            case '2':
               power = 1;
               speed = SP2;
               printf("단");
               break;
            case '3':
               power = 1;
               speed = SP3;
               printf("단");
               break;
            case '4':
               if(w_dir ==1){
                  w_dir = 0;         
               }
               else if(w_dir ==0){
                  w_dir = 1;
               }
            
               break;
                           
         }
         pthread_mutex_unlock(&lock);
         
         fflush (stdout) ;
         }
      delay (10);
   }

}

void *led(){
   while(1) {
      switch (speed)
      {
         case 0:
            digitalWrite (LED1, LOW);
            digitalWrite (LED2, LOW);
            digitalWrite (LED3, LOW);
            break;
         case SP1:
            digitalWrite (LED1, HIGH);
            digitalWrite (LED2, LOW);
            digitalWrite (LED3, LOW);
            break;
         case SP2:
            digitalWrite (LED1, HIGH);
            digitalWrite (LED2, HIGH);
            digitalWrite (LED3, LOW);
            break;
         case SP3:
            digitalWrite (LED1, HIGH);
            digitalWrite (LED2, HIGH);
            digitalWrite (LED3, HIGH);
            break;
      }
   delay (10);
   }

   
}

void *step()
{
   init_Step();
   int angle = 180;

   
  while(1){
 
        if(w_dir ==1)
        {
        
        
         if (curAngle > 0 && curAngle < 180)
         {

            
              one_two_Phase_Rotate_Angle(curAngle, s_dir);
                        
                                      
            printf("%d\n", curAngle);
            fflush (stdout) ;
            
         }
         
         if (s_dir == 1) s_dir = 0;
         else if(s_dir == 0) s_dir = 1;
         
         one_two_Phase_Rotate_Angle(angle, s_dir);   


        }
     }
    
   one_two_Phase_Rotate_Angle(curAngle, s_dir);
   init_Step();
   delay(1000);
  }

 

int main(void)
{   
   if (wiringPiSetupGpio() == -1)
      return -1;

   init();
 
   if (pthread_mutex_init(&lock, NULL) != 0)
   {
      printf("\n mutex init failed\n");
      return 1;
   }

   pthread_t p_thread1;
   pthread_t p_thread2;
   pthread_t p_thread3;
   pthread_t p_thread4;
   pthread_t p_thread5;

   int status=0;

   pthread_create(&p_thread1, NULL, motor, NULL); //dc
   pthread_create(&p_thread2, NULL, ultra, NULL); // 초음파
   pthread_create(&p_thread3, NULL, serial, NULL); //블루투스
   pthread_create(&p_thread4, NULL, led, NULL); // led
   pthread_create(&p_thread5, NULL, step, NULL); //스텝모터
   
   pthread_join(p_thread1, (void **)&status);
   pthread_join(p_thread2, (void **)&status);
   pthread_join(p_thread3, (void **)&status);  
   pthread_join(p_thread4, (void **)&status);
   pthread_join(p_thread5, (void **)&status);


   return 0;

}
