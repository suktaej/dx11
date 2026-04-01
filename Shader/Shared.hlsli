struct PS_OUT_SINGLE
{
    // SV 사용 시 레지스터 번호(0) 생략 가능
    // :(symentic) 원하는 레지스터를 지정
    // 출력 레지스터에 SV_TARGET 값을 삽입
    float4 Color : SV_TARGET;
};