# [GoldKing] Client_YMY (230920)

## Today's Task

  ### 1. 데일리 스크럼 및 JIRA세팅
  
  ### 2. GameManager Framework 세팅 
  - GameMode, PlayerController
  - GameState, PlayerState
  
  ### 3. Random Spawn Point 생성
  - Player 
    - 스폰포인트 위치 지정 후, 랜덤으로 스폰 
    - (중복방지) 효율적인 방법 도입
      - Transform array  
      - get all actors로 모든 스폰포인트의 transform 정보를 담은 배열 생성
      - array 루프돌며 random index와 내용물 교환
        - -> 중복 없는 랜덤 구현 가능 
  - Fountain (랜덤상점)
    - 중복 가능, 모든 정해진 스폰포인트에서 종류만 랜덤으로 스폰되는 방식
    - get all actors로 spawnoint class array 루프 돌며 transform을 받아오고,
    - Select + Index(Random Int)로 종류를 랜덤화하여 스폰 

  ### 4. System UI Layout 
  - 로그인 및 로비, Host/Join UI Layout 생성

-------------------
## 회의록
  - [스크럼 회의](https://gainful-pineapple-5a6.notion.site/21-e02ef9d5eff947d5889d9c0cfe5f3537?pvs=4)
  
## Server 관련 회의 주요내용
  1. 유저 로그인
    - EOS 활용
  2. 세션 Hosting & Joining
    - 조사 중 (문제가 되는 부분)
    - 현재 상태
        - `1`을 누르면 Host가 리슨 서버로 레벨을 생성
        - `2`를 누르면 Client가 지정된 Host IP를 따라 레벨에 접속
            - 단, 빌드 시 Host의 IP를 하드코딩해서 구현해 놓았음 (그래서 레벨을 생성할 수 있는 Host는 하드코딩된 IP를 가진 Host 하나)
  3. 인게임 진행
      - 리슨 서버로 구현
  ---
  SSAFY 내부 환경에서는(LAN 환경) 위의 방식으로 연결이 잘 되지만, WAN 환경에서 연결을 어떻게 해야 할지를 잘 모르겠음. 만약 외부 플랫폼 서비스를 사용하지 않고 연결을 직접 구현한다면 아래의 선택지를 고려해야 함 

---


1. **Hole Punching 방식 및 Relay Server 활용**
    - STUN(Session Traversal Uilities for NAT)으로 Hole Punching 후 안 될 시 TURN 서버로 Relay 방식으로 연결

2. **Hole Punching 방식만**
    - STUN만 활용, 안 된다면 Relay 연결이 아니라 disconnect

3. **Dedicated Server로 구현**
    - 동시접속자 수 제한을 둬야 함 (EC2 스펙 : RAM 16GB / HDD 300GB)

---

1. TURN 서버 사용: STUN으로 해보고 안되면 EC2에 올린 릴레이 서버를 통해 릴레이
    
    ⇒ 서버에 부담이 가긴 하겠지만 5번 만큼은 아님. 딜레이 문제가 있을 수도 있음.
    
2. STUN만 사용: 사용자 간 직접 연결 시도. 따로 릴레이는 구현하지 않음.
3. 유저가 직접 LAN 또는 WAN을 지정: 연결이 될지 안 될지는 보장할 수 없음
4. LAN 연결만 ⇒ 서버 리스로 가겠습니다. API연결은 할 수 있음. 세션 시그널링은 안 됨.
    1. 인터페이스 
    2. 소셜로그인 느낌으로 OpenID? 제공하는 인증제공자? 스팀, 엑스박스 이런 데로 로그인해서 인증을 Epic쪽으로 넘겨줘서 인증 처리한다?
    3. 아마 직접 인증 부여 서버를 만들어야 함 (정상 작동 여부는 모름 OpenID)
5. 데디케이티드 서버로 구현 : 동시접속자 수 제한 둘 것. (16GB/300GB EC2)

----


## TIL 
### [Unreal Engine Multiplayer Framework](https://www.youtube.com/watch?v=pvDgmnxewuk&list=PLNb7FZ2Nw2HTcJ9Qvy8n2Ou-ZVbsDOMFh&index=5)<br>

### [서버 관련 구현방안 회의 및 전문가 리뷰](https://www.notion.so/13-00-2-a7aeabf5796f40a48eb2e4f31504f6b7)

----




## 클라이언트 진행상황  
- UI 작업 중 
<!-- ### 3 Fountains
- <HP / Stats / Gold> Fountain 
    ![img](img/3Fountains.png)
### Use of Each
- HP Fountain 
    ![img](img/HPFountain.png)

- Stats Fountain 
    ![img](img/StatsFountain.png)

- Gold Fountain
    ![img](img/GoldFountain.png)
  골드 계산은 (원래 보유골드 * Random Float - 분수 비용)으로 하였음   -->
  


    
  