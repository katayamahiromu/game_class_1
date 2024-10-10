struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

//スプライト描画単位で指定するパラメータ
cbuffer CBColorGrading : register(b0)
{
	float hueShift; //色相調整
	float saturation; //彩度調整
	float brightness; //明度調整
	float dummy;
};

float3 RGB2HSV(float3 rgb)
{
	float3 hsv = 0;
	float Vmax = max(rgb.r, max(rgb.g, rgb.b));
	float Vmin = min(rgb.r, min(rgb.g, rgb.b));

	float delta = Vmax - Vmin;

	hsv.z = Vmax;

	hsv.y = (delta / Vmax) * step(0, Vmax);

	if (hsv.y > 0.0f)
	{
		if (Vmax == rgb.r)
		{
			hsv.x = 60 * (rgb.g - rgb.b) / delta;
		}
		else if (Vmax == rgb.g)
		{
			hsv.x = 60 * (rgb.b - rgb.r) / delta + 120;
		}
		else if (Vmax == rgb.b)
		{
			hsv.x = 60 * (rgb.r - rgb.g) / delta + 240;
		}

		if (hsv.x < 0)
		{
			hsv.x += 360;
		}
	}
	return hsv;
}


float3 HSV2RGB(float3 hsv)
{
	float3 rgb = 0;
	if (hsv.y == 0)
	{
		rgb.r = rgb.g = rgb.b = hsv.z;
	}
	else
	{
		float Vmax = hsv.z;
		float Vmin = Vmax - (hsv.y * Vmax);
		hsv.x %= 360;
		float Huei = (int)(hsv.x / 60);
		float Huef = hsv.x / 60 - Huei;
		float p = Vmax * (1.0f - hsv.y);
		float q = Vmax * (1.0f - hsv.y * Huef);
		float t = Vmax * (1.0f - hsv.y * (1 - Huef));

		if (Huei == 0)
		{
			rgb.r = Vmax;
			rgb.g = t;
			rgb.b = p;
		}
		else if (Huei == 1)
		{
			rgb.r = q;
			rgb.g = Vmax;
			rgb.b = p;
		}
		else if (Huei == 2)
		{
			rgb.r = p;
			rgb.g = Vmax;
			rgb.b = t;
		}
		else if (Huei == 3)
		{
			rgb.r = p;
			rgb.g = q;
			rgb.b = Vmax;
		}
		else if (Huei == 4)
		{
			rgb.r = t;
			rgb.g = p;
			rgb.b = Vmax;
		}
		else if (Huei == 5)
		{
			rgb.r = Vmax;
			rgb.g = p;
			rgb.b = q;
		}
	}
	return rgb;
}