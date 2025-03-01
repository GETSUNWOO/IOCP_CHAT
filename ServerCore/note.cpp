// db

// 제품군.. window -> 대부분은 sql / mysql
// odbc -> dx처럼 제품군에 관게없이 코딩할 수 있도록 하는 라이브러리

// xmlparser 전까지 sql 로컬서버 db바인딩 작업
// xmlparser -> 버전관리 시작

// db를 사용했을 때 소스코드 버전관리와 꼬일 수 있음
// 매 버젼마다 쿨하게 db를 날리고 만드는건 x
// 
// ORM기술 ex) entity frame work core -> c# (이건 소스코드랑 연동)
//
// 버전관리 두가지 스킬 
// 1. DBA에서 sql 파일들을 직접관리 -> db버젼에 맞추어 서버연동
// 2. db설계를 xml로 해서 관리 (즉 간단한 ORM기술을 구현한다는 느낌)

// xml parsing 필요파일. xmlrapid

// 툴 자동 빌드과정
// make.exe 파일로 실행파일 만들기
// common쪽에 복사
// bat. 파일로 실행 보장 -> 빌드이벤트까지 
// 이 일련의 과정에 보통 파일명과 경로가 들어가기 때문에 확인 필수






//패킷 설계

// 1. 가입 / 2. 로그인 / 3. 채팅

// 1. 가입
// C -> S // name(string) , password(string) -> 서버에서 검증 후 DB 저장
// S -> C // success(bool) -> 이걸 클라가 받아서 가입에 성공했다 실패했다 정도 나타내기
// 

// 2. 로그인
// C -> S // name(string) , password(string) -> 서버에서 DB 탐색 후 성공 시 gamesession에 account 정보 저장
// S -> C // success(bool) -> 성공 수신 후 채팅 화면으로 / 실패 시 알림 / 채팅방내 모든 플레이어에게 현재 플레이어 목록 전달
// 
//

// 3. 채팅
// C -> S // msg(string) -> 서버에서 validation 검사 (글자 수 등)
// S -> C // BroadCasting -> 모든 플레이어에게 chat 패킷 전달

//0221 todo

// 채팅 참여자 목록...... 서버 로그 점검..... 끝
