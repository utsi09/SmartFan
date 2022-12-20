# SmartFan

## 보고서 목차

* 프로젝트 소개: 프로젝트의 목적, 주요 업무 내용, 참가자 명단 등을 소개

* 프로젝트 진행 과정: 프로젝트의 진행 과정을 자세히 설명한다. 핵심 기능과 상이한 일정, 회로 설계, 시스템 구조, 제한조건, 코드의 상세한 설명 등을 기술

* 결과 및 성과: 프로젝트를 수행한 결과를 자세히 설명한다. 모듈 제작과 결과물, 시연 영상 등을 기술 

* 요약: 프로젝트의 전반적인 요약을 기술

# 프로젝트 소개

## 참가자 명단

* 20190703_엄태욱

* 20170766_이건우

* 20191333_김영호

## 프로젝트의 목적

에어컨에 리모콘 있는 경우는 많지만 선풍기가 리모콘을 통해서 사용자와 멀리서 상호 작용을 하는 경우가 거의 없고 불편함.

사용자는 해당 선풍기가 어느정도의 풍속으로 돌고있는지 확인하기 힘들다는 불편함이 존재함.

가정에 어린 자녀가 있는 경우 선풍기가 안심망이 있지만, 호기심에 안전망 속으로 손가락을 넣는경우의 안전사고가 발생할 가능성이 있음.


## 주요 업무 내용

* 선풍기의 내부 프로그램에 해당하는 코드 작성

* 선풍기의 작동을 위한 많은 모듈들의 회로 작성 

* 실제 선풍기처럼 구동하기 위한 오브젝트 제작

* 사용자들의 버그없는 원할한 사용을 위한 디버깅

# 프로젝트 진행 과정

## 일정 

<center><img src="https://user-images.githubusercontent.com/92018822/208082204-2ec02b48-26b1-4230-9312-4749692603b3.png" width="600" height="400"></center>

팀의 프로젝트 일정은 위 이미지와 같이 간트차트로 정리하였음.

## 핵심 기능


* Push 버튼으로 선풍기의 전원 On/Off 가능

* 블루투스를 사용해 스마트폰으로 선풍기의 전원, 풍속, 풍향 제어가 가능

* 풍속은 3단까지 조절 가능하며, 불이 켜진 LED의 개수로 풍속을 확인 가능

* 초음파 센서로 가까이 접근하는 물체를 감지해 프로펠러를 자동으로 멈추기 기능

* 프로펠러를 멈추기 전부터 접근하는 물체에게 부저를 통해 경고음 발생 기능

## 시스템 구조

<center><img src="https://user-images.githubusercontent.com/92018822/208086434-f8dd63bd-407b-40d1-b035-a9a64ed67690.png" width="800" height="400"></center>

시스템은 다음과 같은 구조를 가지고 있음

* 블루투스 모듈
* 푸쉬버튼
* 초음파 센서
* 소리 부저
* LED
* 스텝모터 드라이브
* 스텝 모터
* DC모터 

시스템에 사용된 모듈로, 총 8개의 모듈이 사용됨.

## 회로 설계

<center><img src="https://user-images.githubusercontent.com/92018822/208218873-c93aa97d-576b-48a5-a70a-ae3dd7658ba3.png" width="800" height="400"></center>

위 이미지와 같이 회로를 설계하였음

## 제한 조건

제한 조건중 멀티쓰레드, 뮤텍스를 구현하였음.

모터 함수에서 구현된 뮤텍스

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

   
시리얼 함수에서 구현된 뮤텍스

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


쓰레드 구현을 위해 5개의 쓰레드 함수가 존재함

           void *motor()
           
           void *ultra()
           
           void *serial()
           
           void *led()
           
           void *step()
          
이 함수들로 이루어진 멀티 쓰레드를 구성

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
   
   
   
   ## 코드 설명
   
   
   ### 변수 설명
   
           int power = 0;
           int speed = 0, dir=1;
           int w_dir = 0;
           int s_dir = 0;
           int curAngle = 0;

   * power : 선풍기의 전원을 관리 
   * speed : 선풍기의 풍속을 관리
   * dir : DC모터의 프로펠러의 회전 방향
   * W_dir : 풍향을 담당하는 스텝모터의 전원을 관리
   * s_dir : 스텝모터가 회전 중인 현재 방향
   
   ### 풍속 정의
   
           #define SP1 33
           #define SP2 66
           #define SP3 100
         
* 풍속 1단계 : SP1
* 풍속 2단계 : SP2
* 풍속 3단계 : SP3


### 초기화 과정

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
           
* 모듈들의 핀 입출력을 초기화

* 종료전의 스텝모터의 각도 위치 정보와 진행중이던 회전 방향을 텍스트파일에서 Read

## motor 함수

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
           


DC 모터의 회전을 담당하는 함수이다.

버튼이 눌렸을때의 전원을 켜고 끄는 기능이 포함되어 있다.

power값이 1이 되었을 때 motor_rotate 함수에 현재 설정된 speed 와 dir을 넘겨주어 회전시키고,

power 값이 0이 되었을 경우엔 speed 를 0 으로 설정해준다

## ultra 함수

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



초음파 센서의 거리를 감지하여 액션을 취하는 함수이다.

초음파 센서에 20cm 이상 30cm 이하의 거리의 물체가 감지되었을 경우

딜레이가 큰 느린 경고음을 소리 부저를 통해 발생시키고

초음파 센서에 20cm 이하의 거리의 물체가 감지된 경우에는

딜레이가 작은 빠른 경보음을 소리 부저를 통해 발생시키고

* 정지전의 Speed를 tempSpeed에 임시 저장

* 정지전의 Power를 tempPower에 임시 저장한후

DC모터가 돌던 방향의 반대의 방향으로 힘을 가해 급정지가 가능하도록 한다.

	     if(tempSpeed == SP1){
                   delay(250);
                }
                else if(tempSpeed == SP2){
                   delay(230);
                }
                else if(tempSpeed == SP3){
                   delay(150);
                }
다음과 같이 풍속별로 딜레이 값을 다르게 주어 풍속에 맞는 정지를 할 수 있도록 하였다.



## serial 함수

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
           
           
스마트폰과 통신을 위한 쓰레드이다. 블루투스를 시리얼 통신을 통해 스마트폰으로부터 값을 입력 받는다. 입력 받은 값에 따라서 경우가 나뉘어 지는데, 

0의 경우
- 선풍기의 전원을 작동 시킨다. 전원이 On일 경우 Off로, Off일 경우 On으로 바꾸어 준다. 동시에 풍속, 풍향 변수를 0으로 초기화 한다.

1의 경우
- 선풍기의 전원을 켠 후 풍속을 1단계로 설정한다.

2의 경우
- 선풍기의 전원을 켠 후 풍속을 2단계로 설정한다.

3의 경우
- 선풍기의 전원을 켠 후 풍속을 3단계로 설정한다.

4의 경우
- 선풍기의 풍향을 설정한다. 선풍기가 회전하고 있으면 선풍기를 고정시키고, 선풍기가 고정되어 있으면 선풍기를 회전시킨다. 

## led 함수

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


<led 스레드>

풍속을 LED로 표시해 주기 위한 쓰레드이다. 풍속에 따라 불이 들어오는 LED 개수가 달라진다.

전원이 꺼져 있을 경우(풍속이 0인 경우)
- LED 3개를 모두 끈다.

풍속이 1단인 경우
- LED 1개는 켜고, 2개는 끈다

풍속이 2단인 경우
- LED 2개는 켜고, 1개는 끈다

풍속이 3단인 경우
- LED 3개를 모두 켠다.

## one_two_Phase_Rotate_Angle 함수

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

각도와 풍향을 받아서 스텝모터를 회전 시켜주는 함수이다.


## step 함수 

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

선풍기를 회전 시키기 위한 쓰레드이다. 

선풍기의 회전을 작동 시키면, 선풍기는 0~180 사이에서 좌우로 회전한다. 

회전 중에 선풍기를 고정 시키면 선풍기는 그 자리에 고정되고, 다시 회전 시키면 전에 하던 회전에 이어서 회전한다.


# 결과 및 성과

## 모듈 제작


선풍기는 인간의 편의에 기반하여 발명된 기기이므로 제작을 하면서 최대한 사용자의 입장에서 불편하지 않게 제작하였다.

우선 선풍기의 프로펠러의 위치를 높일 장치가 필요하였다. 

스마트폰 거치대는 스마트폰의 높이를 사용자의 눈에 최적화되게 해주므로 

사용자의 얼굴쪽에 바람을 쐬어줄 수 있는 높이를 제공하는 지지대로서 적합하다고 생각되었다.

또한 원래 스마트폰을 고정해야하는 부분은 보드를 구비하여 스텝모터를 튼튼하게 지지하게 하였다.

DC모터에는 컵홀더와 연결하여 컵홀더 윗부분에 초음파 센서를 부착하였다. 


DC모터와 스텝모터의 결합이 가장 어려웠던 부분이었는데

많은 실패 케이스가 있었다.

* 스텝모터와 컵홀더를 부착
* 스텝모터에 철사를 감아 컵홀더에 고정
* 양면테이프로 고정

하지만 고정력이 약하거나 철사가 풀리는 문제가 발생하였다.


해결법은 다음과 같았다.

* 스텝 모터위에는 플라스틱 병뚜껑에 구멍을 뚫어 연결하고

* 병뚜껑에 옆에 구멍을 뚫어 철사를 연결하여 철사를 통해 컵홀더를 부착하였다. 

이 해결법을 통해 스텝모터의 회전이 헛돌지 않고 스텝모터의 회전이 온전히 프로펠러의 회전으로 이어지게 할 수 있었다. 


기존에는 프로펠러에 L화일을 프로펠러 모양으로 오려 붙였지만 L화일의 무게와 공기저항 때문에 

바람의 범위는 넓어졌지만 풍속이 느려져 아쉬움이 있었고

최종 데모에는 추가 프로펠러를 제거하였다.


그다음 지지대에 라즈베리파이와 브레드보드를 부착하여 하나의 모듈로 만들어 선풍기의 이동을 간편히 할 수 있었다.

또한 수많은 선은 테이프로 보기좋게 정리하였고 실제 사용자의 편의에 신경 쓴 모듈 제작에 성공할 수 있었다.

## 결과물

<center><img src="https://user-images.githubusercontent.com/92018822/208663070-27f23bc9-74eb-4112-8760-96acdcb8c129.jpg" width="500" height="700"></center> 

<center><img src="https://user-images.githubusercontent.com/92018822/208662919-df743886-1f1b-404b-8db5-483cf2146413.jpg" width="500" height="700"></center>

<center><img src="https://user-images.githubusercontent.com/92018822/208662681-a61f8b1a-6e90-4e34-a3a5-96453225d69d.jpg" width="500" height="700"></center>

수많은 시행착오를 통해 깔끔한 스마트 선풍기를 제작할 수 있었다. 

버튼과 LED, 블루투스 모듈은 선풍기의 정면에 있고 

가려져있지만 스마트폰 거치대를 사용한 만큼 지지대 부분을 늘려 선풍기 바람이 나오는 부분을 조정할 수 있다.


## 시연 영상

