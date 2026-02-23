## 🎮 프로젝트 시연
- 시연 영상: <시연 링크>  
  https://www.youtube.com/watch?v=sr4bcuqVucU
<img width="600" height="400" alt="image" src="https://github.com/user-attachments/assets/2d908594-d6ae-4e80-8655-a05132da19ae" />


## 🔧 핵심 구현 기술

### 1. Gameplay Ability System(GAS) 기반 확장형 전투 아키텍처
- 언리얼 엔진의 고도화된 스킬 시스템인 GAS를 전면 도입하여, 기본 이동을 제외한 대시, 스킬 공격, 피격, 사망 처리를 모두 어빌리티(Ability) 단위로 모듈화하고 확장성 있는 전투 구조를 설계했습니다.
- GameplayTag를 적극 활용하여 캐릭터의 상태 판정, 스킬 분기, 상호작용 조건 등을 데이터 기반(Data-Driven)으로 통제함으로써, 하드코딩을 배제하고 복잡한 전투 로직의 결합도를 낮추었습니다.

### 2. 로직과 연출의 분리 및 데이터 주도적 스탯 관리
- AttributeSet과 GameplayEffect를 연동하여 스킬에 따른 자원 소모, 데미지 계산, 상태 이상 효과 적용 로직을 독립적인 데이터 자산으로 분리해 재사용성을 극대화했습니다.
- GameplayCue를 활용하여 스킬 발동 시점의 시각적 이펙트(VFX) 및 청각적 연출(SFX)을 핵심 논리 코드와 완벽히 분리 제어함으로써, 네트워크 환경에서도 안전하고 깔끔한 코드 구조를 유지했습니다.

### 3. Behavior Tree 기반 체계적인 AI 시스템 로직 구성
- 언리얼 엔진의 Behavior Tree와 Blackboard를 활용하여 적 AI의 상황 인지, 추적, 공격 패턴을 유기적으로 연결하는 행동 트리 로직을 구축했습니다.
- 플레이어의 피격 및 사망 상태, 거리 진입 여부 등의 Blackboard 데이터를 기반으로 AI가 적절한 스킬 태스크를 수행하도록 설계하여 역동적인 싱글 플레이 전투 환경을 구현했습니다.

### 4. Listen Server 기반 멀티플레이어 동기화 및 서버 권한 검증
- Listen Server 및 Dedicated Server 환경을 고려하여 네트워크를 구성하고, 클라이언트의 임의 변조를 방지하기 위해 핵심 판정 로직을 서버 권한(Server Authority) 기반으로 안전하게 설계했습니다.
- 언리얼 네트워크 레플리케이션(Network Replication)을 적용하여, 분리된 클라이언트 환경에서도 캐릭터의 이동, 애니메이션, 피격 상태 등이 지연 없이 정확하게 동기화되도록 구현했습니다.
