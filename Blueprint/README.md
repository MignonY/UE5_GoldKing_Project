# 블루프린트 정리
## Index
게임 전체에 대한 관리를 담당하는 GameInstance는 레벨이 많지 않아 큰 활용내역이 없었음 

### 0. GameMode 
- [플레이어 정보 수집] 및 [스폰 / 부활] 구현
  - 중복 없는 플레이어 랜덤 스폰 구현
    - 전체 player 숫자 이상의 크기를 가지는 Array setting
    - Loop 한바퀴를 돌며, index 숫자 범위 내 random num과, 현재 index의 숫자를 바꾸어 각 index에 삽입함. 
    - 이후 player들에게, 뒤섞인 array의 앞 index부터 내용물 번호 부여 
    - 한바퀴만 돌며 random num과 교환 과정을 거침으로써 불필요한 반복이나 연산을 줄임
  - 중복을 허용한 랜덤 분수 스폰 구현
    - (분수 3종) class형식의 Array Setting (index 0-2)
    - spawn시 class의 array에서 
### 1. GameState 
- 게임 룰 관련 변수 및 로직 구현
### 2. CharacterBP (캐릭터 정의)
- 컨트롤러/HUD 연결, 장비 및 데미지 관련 로직 구현
### 3. PlayerState (플레이어 상태관리)
- 플레이어 상태, 재화 관련 변수 및 로직 구현
### 4. PlayerController (입력 / 로컬 관련 처리)
- 로컬 입력 및 UI 관련 처리 
### 5. KeyMapping (단축키-행위)
- Input Mapping
### 6. Interact (상호작용)
- 상호작용 로직 및 UI (G키) 연결 
### 7. NPC (상인)
- 무기상점 및 랜덤분수 관련 로직
- 상점 아이템 관련 로직 및 UI
### 8. Boss & Monster (보스, 몬스터 AI)
### 9. UI (로비, 시스템 및 HUD)
- System UI / HUD / Alert 디자인 및 구현 