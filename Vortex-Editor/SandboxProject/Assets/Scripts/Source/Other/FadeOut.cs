﻿using System;
using Vortex;

namespace Sandbox {

	public class FadeOut : Entity
	{
		public float DelayStart;
		public float DelayEnd;
		public float DisplayTime;
		private float TimeRunning;
		private SpriteRenderer m_Sprite;
		public bool Over = false;

		protected override void OnCreate()
		{
			m_Sprite = GetComponent<SpriteRenderer>();
		}

		protected override void OnUpdate(float delta)
		{
			if (DelayStart > 0.0f)
				DelayStart -= delta;
			else
			{
				Vector4 color = m_Sprite.Color;
				color.W -= delta * 0.37f;

				TimeRunning += delta;

				if (TimeRunning > DisplayTime)
					color.W += delta;

				if (color.W < 0.0f)
					color.W = 0.0f;
				else if (color.W > 1.0f)
				{
					color.W = 1.0f;
					Over = true;
				}

				if (Over)
					DelayEnd -= delta;

				if (DelayEnd <= 0.0f)
					SceneManager.LoadScene("Boost_01");

				m_Sprite.Color = color;
			}
		}
	}

}
