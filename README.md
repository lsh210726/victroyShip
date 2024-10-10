## 프로젝트 소개

식물에게 칭찬을 해서 성장시킨 후 npc에게 선물, 대화를 통해 호감도를 높이는 힐링 게임입니다.

[![팜라이프 채팅 이미지](https://github.com/lsh210726/victroyShip/blob/main/8RsrgYo98IE%2000-03-24%20%282%29%20%EC%9A%B0%EC%8A%B9%ED%98%B8%20%ED%8C%9C%EB%9D%BC%EC%9D%B4%ED%94%84%ED%94%8C%EB%A0%88%EC%9D%B4%EC%98%81%EC%83%81.png)](https://youtu.be/8RsrgYo98IE?si=YPuazwJks8IeqZYP&t=174)
https://youtu.be/8RsrgYo98IE?si=YPuazwJks8IeqZYP&t=174

개발인원 : AI 1, 클라이언트 3, 레벨디자인 1  
개발기간 : 24/5/8 ~ 24/6/25 (7주)  

2024 메타버스 엑스포 전시

---
### 개발한 것
언리얼 엔진과 LLM 챗봇을 접목시켜 실시간 NPC 대화를 구현하였습니다.  

언리얼 클라이언트와 파이썬 챗봇 간의 정보 전달 기능과 NPC 롤플레잉 챗봇을 만들었습니다.
 1. 언리얼-파이썬 인터페이스 개발
 2. 캐릭터 챗봇 구현
 3. 챗봇 메모리 구현
 4. 캐릭터 TTS 구현
---
### 사용기술
- 언리얼 엔진 5.4
- Unreal Blueprint
- C++
- Python
- GPT 4o
- RAG
- FastAPI

## 챗봇 아키텍처
![챗봇 아키텍처](https://github.com/lsh210726/victroyShip/blob/main/farmlifeAI%20(1).jpg?raw=true)
## 메모리-세션
API 챗봇은 기본적으로 기억을 하지 않기 때문에 이전 대화 내용을 저장해주기 위해 각 npc별로 세션을 만들었습니다.


대화를 나누는 npc이름을 매개변수로 세션을 검색합니다. 만약 세션이 없는 경우 새로 생성합니다.  


   ```mermaid
flowchart LR
    A["<b>시작</b>"] --> B{"<b>NPC 이름으로<br/>세션 검색</b>"}
    B -->|"<b>세션 없음</b>"| C["<b>새 세션 생성</b>"]
    B -->|"<b>세션 있음</b>"| D["<b>기존 세션의<br/>대화 기록을<br/>프롬프트에 삽입</b>"]
    C --> E["<b>대화 시작</b>"]
    D --> E
    E --> F["<b>대화 진행</b>"]
    F --> G["<b>세션 업데이트</b>"]
    G --> H["<b>종료</b>"]

    classDef default fill:#f9f9f9,stroke:#333,stroke-width:2px,color:black,font-size:14px;
    classDef process fill:#d4f1f4,stroke:#05a8aa,stroke-width:2px,color:black,font-size:14px;
    classDef decision fill:#ffe6ab,stroke:#ffa62b,stroke-width:2px,color:black,font-size:14px;
    classDef start_end fill:#ffb3ba,stroke:#a83e32,stroke-width:2px,color:black,font-size:14px;
    
    class A,H start_end;
    class B decision;
    class C,D,E,F,G process;
    
    linkStyle default stroke:#2b59c3,stroke-width:2px;
   ```


MessagesPlaceholder을 사용하면 자동으로 입출력이 저장되지만 이번 프로젝트에서는 챗봇 출력이 대화문 외 감정이나 호감도 등의 값이 같이 출력되므로 ChatMessageHistory를 통해 대화문 내용만 세션에 저장시킵니다.

<details>
<summary>코드</summary> 

 
```python
session_store  = {} # 메시지 기록(세션)을 저장할 딕셔너리

  

def  getChatLog(session_ids : str):#npc 개별로 채팅 기록 생성, 각각 기존의 채팅 내역을 기억하고 대화에 반영함.

	if  session_ids  not  in  session_store: # 세션 기록이 없을 경우 - 유저가 대화한 적이 없을 경우 -> 새 채팅창 생성

		session_store[session_ids] = ChatMessageHistory()# 새로운 객체 생성해서 세션 스토어에 저장하기

	return  session_store[session_ids] # 대화한 적이 있을 경우 저장된 세션 ID 기록 반환하기

  

  

def  talk2npc(npcName:str,dialog:str,preperence:int):#이름, 대화문, 현재 호감도

	getChatLog(npcName).add_user_message(dialog) #유저의 입력을 로그에 입력

	###챗봇과 대화 처리 코드###

	getChatLog(npcName).add_ai_message(response.answer)#결과 중 대답 부분만 로그에 저장
```

</details>


## PydanticOuputParser을 이용한 출력 고정
챗봇에게서 단순 응답만 생성하는 것이 아닌, 대화 내용에 따라 변하는 NPC의 감정도 함께 생성시켜 더 사실적인 NPC를 구현했습니다.  

<img src="https://github.com/lsh210726/victroyShip/blob/main/chatPrompt1.jpg" alt="chatPromptImg" style="width: 30%;">  

출력들 중 필요한 데이터만 추출해야 하기에 챗봇의 출력을 PydanticOuputParser을 사용하여 정형화하였습니다.
<details>
<summary>코드</summary>

```python
# pydantic 모델 설정 - 출력양식

class  GameRequest(LangChainBaseModel):

	answer: str  =  Field(description="요청에 대한 답변")

	imotion: str  =  Field(description="Respond to emotional changes caused by talking to humans with either joy, surprise, sadness, or anger.")

	#human과의 대화로 인한 감정의 변화를 기쁨, 놀람, 슬픔, 화남 중 하나로 응답할 것

	likability: int  =  Field(description="Respond to the change in favorability from talking to a human as an integer between -10 and 10.")

	#human과의 대화로 인한 호감도의 변화량을 -10 에서 10 사이의 정수로 응답할 것

parser  =  PydanticOutputParser(pydantic_object=GameRequest)# parser 설정

prompt  =  prompt.partial(format=parser.get_format_instructions())# 프롬프트 설정
```
</details>


단순한 응답이 아닌 응답과 감정, 호감도 변화를 같이 출력시킵니다.  
생성된 감정에 따라 게임 캐릭터의 이미지와 애니메이션이 변경됩니다.  
생성된 호감도 변화치에 따라 캐릭터의 현재 유저에 대한 호감도가 변경됩니다. 


<div style="display: flex; justify-content: center;">
    <img src="https://github.com/lsh210726/victroyShip/blob/main/8RsrgYo98IE%2000-03-23%20(2)%20farmlifeSurp.png" alt="Image 1" style="width: 45%; margin-right: 10px;">
    <img src="https://github.com/lsh210726/victroyShip/blob/main/8RsrgYo98IE%2000-03-54%20(2)%20farmlifeSmile.png" alt="Image 2" style="width: 45%;">
</div>

## 프롬프트 템플릿
챗봇에 대화 요청 시 자연스러운 대화 생성을 위해 대화문 요청 프롬프트에 사용자의 대화문과 함께 NPC의 페르소나, 현재 사용자와의 호감도에 맞는 대화 예시문, 출력 포맷 등을 포함하여 전달합니다.


<img src="https://github.com/lsh210726/victroyShip/blob/main/chatPrompt.jpg" alt="chatPromptImg" style="width: 30%;">


<details>
<summary>코드</summary>

 
```python
#프롬프트 내용 : 당신은 감정을 가진 사람입니다. 당신의 페르소나 {persona}을 이해하고, 이전 대화 기록과 예시 대화를 바탕으로 주어진 요청에 한국어로 200자 이내의 대화 스타일로 응답하세요. {dialogue_example}
template  =  """
You are a person with feelings. Understand your persona, {persona}, and respond to a given request in a conversational style of no more than 200 characters in Korean, based on previous dialogue history and example dialogue. {dialogue_example}

request:
{request_content}

FORMAT:
{format}
"""

prompt  =  PromptTemplate.from_template(template=template)
```
</details>


## 호감도에 따른 말투 변경
사용자와 npc 간의 호감도에 따라 대화 예시문이 변경됩니다. 
npc와 플레이어 간의 호감도가 올라갈수록 더 친밀한 말투를 사용하도록 하여 몰입감 있는 대화가 가능합니다.
<details>
<summary>코드</summary>

 
```python
"호감도 낮은 경우 (낯선 사람 또는 처음 만난 사람)": [

	"안녕하세요, 김준서입니다. 여기 오신 건 처음이신가요?",

	"오늘 날씨가 참 좋네요. 벌통 관리할 때 이런 날씨가 제일 좋아요.",

"호감도 중간 (친해지고 있는 이웃 또는 지인)": [

	"어제 그 새 사진 찍은 거 봤어요? 정말 잘 나왔더라고요.",

	"최근에 새로운 꿀벌 품종을 시도해 보고 있는데, 이야기 나누고 싶어요.",

"호감도 높은 경우 (친한 친구 또는 가까운 사람)": [

	"야, 오늘은 내가 만든 특별한 꿀벌 샌드위치를 준비했어. 맛있게 먹어!",

	"너한테만 보여주는 건데, 이번에 찍은 새 사진 중에 정말 멋진 거 있어. 같이 보자.",
```
```python
def  set_preference(intPref:int):

	if  intPref  >  70:

		return  "호감도 높은 경우 (친한 친구 또는 가까운 사람)"

	elif  intPref  >  30:

		return  "호감도 중간 (친해지고 있는 이웃 또는 지인)"

	else:

		return  "호감도 낮은 경우 (낯선 사람 또는 처음 만난 사람)"
```
</details>


## 회고
첫 llm 프로젝트였기 때문에 학습과 병행하면서 개발을 진행하다 보니 시행착오가 많았습니다.  
npc의 행동을 llm을 통해 결정하는 기능을 서버에서 구현했으나 시간상의 문제로 프로젝트에서 구현하지 못하여 아쉽습니다. 
