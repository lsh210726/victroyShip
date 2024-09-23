## 프로젝트 소개

식물에게 칭찬을 해서 성장시킨 후 npc에게 선물, 대화를 통해 호감도를 높이는 힐링 게임입니다.

[![팜라이프 채팅 이미지](https://github.com/lsh210726/victroyShip/blob/main/8RsrgYo98IE%2000-03-24%20%282%29%20%EC%9A%B0%EC%8A%B9%ED%98%B8%20%ED%8C%9C%EB%9D%BC%EC%9D%B4%ED%94%84%ED%94%8C%EB%A0%88%EC%9D%B4%EC%98%81%EC%83%81.png)](https://youtu.be/8RsrgYo98IE?si=YPuazwJks8IeqZYP&t=174)
https://youtu.be/8RsrgYo98IE?si=YPuazwJks8IeqZYP&t=174

개발인원 : AI 1, 클라이언트 3, 레벨디자인 1  
개발기간 : 24/5/8 ~ 24/6/25 (7주)  
2024 메타버스 엑스포 전시

---
### 개발한 것
팀장을 맡아 프로젝트를 기획하고 총괄하였습니다.  
언리얼 엔진과 LLM 챗봇을 접목시켜 실시간 NPC 대화를 구현하였습니다.  
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
npc이름을 매개변수로 세션을 검색합니다. 만약 세션이 없는 경우 새로 생성합니다.
```
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
MessagesPlaceholder을 사용하면 자동으로 입출력이 저장되지만 이번 프로젝트에서는 챗봇 출력이 메세지 외 감정이나 호감도 등의 값이 같이 출력되므로 ChatMessageHistory를 통해 메세지 값만 세션에 저장시킵니다.

## PydanticOuputParser을 이용한 출력 고정
챗봇의 응답을 엔진이 이해 가능한 형식으로 정형화 하기 위해 PydanticOuputParser을 사용했습니다.  


```
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
단순한 문자열 응답이 아닌 답변과 감정, 호감도 변화를 같이 출력시킵니다.  
생성된 감정에 따라 게임 캐릭터의 이미지와 애니메이션이 변경됩니다.  
생성된 호감도 변화치에 따라 캐릭터의 현재 유저에 대한 호감도가 변경됩니다. 
## 프롬프트 템플릿
사용자의 대화내용과 캐릭터의 페르소나 및 호감도에 따른 말투, 출력포맷 등을 같이 전달합니다.
```template  =  """

You are a person with feelings. Understand your persona, {persona}, and respond to a given request in a conversational style of no more than 200 characters in Korean, based on previous dialogue history and example dialogue. {dialogue_example}

  

request:

{request_content}

  

FORMAT:

{format}

"""

prompt  =  PromptTemplate.from_template(template=template)
```

## 호감도에 따른 말투 변경
사용자와 npc 간의 호감도에 따라 대화 예시문이 변경됩니다. 플레이어는 npc의 응답이 호감도에 따라 변하면서 더 다양한 경험이 가능합니다.
```
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
```
def  set_preference(intPref:int):

	if  intPref  >  70:

		return  "호감도 높은 경우 (친한 친구 또는 가까운 사람)"

	elif  intPref  >  30:

		return  "호감도 중간 (친해지고 있는 이웃 또는 지인)"

	else:

		return  "호감도 낮은 경우 (낯선 사람 또는 처음 만난 사람)"
```


