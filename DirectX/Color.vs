// ���� ����(Vertex Buffer)

// 3D ���� ������ ���鰳�� �ﰢ������ ������� �ִµ� �� �ﰢ������ �� ��������
// ������ �ְ� �� �������� ����(Vertex)��� �θ���. 
// ���� ���� ǥ���ϱ� ���ؼ� �츮�� ���� �����ϰ� �ִ� ��� �������� 
// ���� ���۶�� �θ��� Ư���� ������ �迭�� �־��־�� �Ѵ�.
// ������ ��� ���� ���� ���ۿ� �־��ٸ� �̸� GPU���� ���� ���� �׸����� �ϰ� �� �� �ֽ��ϴ�.

// �ε��� ����(Index buffers)

// �ε��� ���۴� ���� ���ۿ� ������ �ִµ� �� �ε��� ���۴� �� �������� ����Ǵ�
// ������ ����ϱ� ���� ���̴�. �׷��� GPU�� �ε��� ���۸� �̿��� ������ ����
// ���ۿ��� �ʿ��� ��� �������� ã�Ƴ���. �ε��� ���۴� ���ϴ� ���� ������ 
// ã�°��� �����شٴ� ������ å�� ������ ���ٰ� �� �� �ִ�. 
// DX SDK ���������� �ε��� ������ ����� ���� �޸𸮿��� ���� �������� ĳ����
// ���ɼ��� �����شٰ� �Ѵ�. ���� ���� ���鿡���� �ε��� ������ ����� ����ȴ�.

// ���� ���̴�(Vertex Shaders)

// ���� ���̴��� �ַ� ���� ���۵��� �������� 3D �������� ��ȯ�����ִ� ���� ���α׷��̴�.
// �� �ܿ��� �� ������ ������ ����Ѵٴ��� �ϴ� �ٸ� ���굵 �����ϴ�.
// ���� ���̴� ���α׷��� GPU���� ����� �ʿ��ϴٰ� �Ǵ޵� �� ȣ��ȴ�. 
// ���� ���, 5000���� �������� ���� ���� ȭ�鿡 ǥ���Ѵٸ� ���� �� �ϳ��� ����
// �׸��� ���� �� �����Ӹ��� 15000���� ���� ���̴� ���α׷��� ����ȴ�. ����
// ���α׷��� 60fps�� ���� �׷��� ���α׷������� ���� 5000���� �ﰢ���� �׸��� ����
// �� �ʸ��� 900,000���� ���� ���̴��� ȣ���ϰ� �˴ϴ�. ���� ȿ������ ���� ���̴��� �ۼ��ϴ� ���� �߿��մϴ�.

// �ȼ� ���̴�(Pixel shaders)

// �ȼ� ���̴��� �׸����� �ϴ� ������ ������ ���� ���� ���� ���α׷��Դϴ�.
// �̰��� ȭ�鿡 �������� ��� �ȼ��鿡 ���� GPU���� ����˴ϴ�. ������ ������,
// �ý��ĸ� ������, ���� ȿ���� �ְ�, �� �� �ٸ� ���� ���� ä�� ȿ���� �ִ� ����
// �ٷ� �� �ȼ� ���̴� ���α׷����� ����˴ϴ�. �ȼ� ���̴��� GPU�� ���� ������
// ���� ȣ��Ǳ� ������ �ݵ�� ȿ�������� �ۼ��Ǿ�� �մϴ�.

// HLSL(High Level Shader Language)

// HLSL�� �ռ� ������ DX11���� ����ϴ� ���� ���� �� �ȼ� ���̴� ���α׷��� �ۼ��� ��
// ����ϴ� ������ ����Դϴ�. ������ �̸� ���ǵ� Ÿ���� �ִٴ� ���� ���� C���� ����մϴ�.
// HLSL ���α׷� ������ ���� ����, Ÿ�� ����, ���� ���̴�, �׸��� ���� ���̴��� �����Ǿ� �ֽ��ϴ�.


////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////

// cbuffer�� ��κ��� ���������� �־��ش�.
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// ����� ���� Ÿ��.

struct VertexInputType // <- �������̴�
{
	float4 position : POSITION; // float 4�� HLSL ����Ÿ��.
	float4 color : COLOR; // float 4���� RGB + alpha(������)�� ����ִ�.
}; 
// POSITION �׸��� COLOR �׸��� SV_POSITION�� GPU�� �� �������� ����� ���̶�� �����մϴ�.
// ������ ������ ���� �� �ȼ� ���̴��� �ǹ̰� �ٸ��� ������ �̿� �ش��ϴ� �ΰ��� ����ü�� ��������ϴ�.
// COLOR�� �� ����ü ��� �������� ������ ������, POSITION�� ���� ���̴��� SV_POSITION�� �ȼ� ���̴�����
// �����մϴ�.

struct PixelInputType  // <- �ȼ����̴�
{
	float4 position : SV_POSITION; 
	float4 color : COLOR;
};

////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader