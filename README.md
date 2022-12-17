# SmartFan

## 보고서 목차

* 프로젝트 소개: 프로젝트의 목적, 주요 업무 내용, 참가자 명단 등을 소개

* 프로젝트 진행 과정: 프로젝트의 진행 과정을 자세히 설명한다. 핵심 기능과 상이한 일정, 회로 설계, 시스템 구조, 제한조건 등을 기술

* 결과 및 성과: 프로젝트를 수행한 결과를 자세히 설명한다. 계획된 결과와 실제 결과, 인적/물적 자산과의 연계와 코드의 상세한 설명, 작동원리 등을 기술 

* 요약 및 제안: 프로젝트의 전반적인 요약과 추후 계획을 기술

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
   
   
