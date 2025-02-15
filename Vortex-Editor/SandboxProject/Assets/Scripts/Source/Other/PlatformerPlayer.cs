﻿using System;
using Vortex;

namespace Sandbox {

	public class PlatformerPlayer : Entity
	{
		public float AnimationWaitTime = 1.0f;
		public float JumpForce;
		public float PlayerResetYAxis = -50.0f;
		public float FinishPoint;
		public float HorizontalFriction = 2.0f;
		public float Speed;
		public float WaitTime;
		public float ControllerDeadzone = 0.1f;
		public Vector3 StartPosition;
		public Vector3 Velocity;

		public bool IsGrounded;
		public bool AnimationReady;
		public bool IsRunning;
		public bool ShowRaycast;
		public bool GameOver;

		private Texture2D m_NormalTexture;
		private Texture2D m_JumpTexture;
		
		private RigidBody2D m_Rigidbody;
		private BoxCollider2D m_BoxCollider;
		private SpriteRenderer m_Sprite;
		private AudioSource m_JumpSound;

		private Camera2D m_CameraEntity;

		protected override void OnCreate()
		{
			m_Rigidbody = GetComponent<RigidBody2D>();
			m_BoxCollider = GetComponent<BoxCollider2D>();
			m_Sprite = GetComponent<SpriteRenderer>();
			m_JumpSound = GetComponent<AudioSource>();

			StartPosition = transform.Translation;

			m_CameraEntity = Scene.FindEntityByName("Camera").As<Camera2D>();

			m_NormalTexture = new Texture2D("Textures/game/Platformer/Characters/character_0000.png");

			Log.Info("Platformer Player is loose!");
		}

		protected override void OnUpdate(float delta)
		{
			Vector2 playerFootPoint = new Vector2(transform.Translation.X, (transform.Translation.Y - m_BoxCollider.Size.Y) + m_BoxCollider.Offset.Y);
			Vector2 groundPoint = playerFootPoint;
			groundPoint.Y -= 0.1f;

			Entity entity = Physics2D.Raycast(transform.Translation.XY, groundPoint, out RaycastHit2D hit, ShowRaycast);
			IsGrounded = hit.Hit;

			if (IsGrounded && hit.Tag == "Grass Collider")
			{
				m_Sprite.Color = new Vector4(0.8f, 0.4f, 0.4f, 1.0f);
				Destroy(entity);
			}

			if (transform.Translation.Y <= PlayerResetYAxis)
				SceneManager.LoadScene("Platformer");

			if (Input.IsKeyDown(KeyCode.Escape) || Input.IsGamepadButtonDown(GamepadButton.Start))
				Application.Quit();

			if (Input.IsKeyDown(KeyCode.A) || Input.GetGamepadAxis(GamepadAxis.LeftX) < -ControllerDeadzone)
			{
				IsRunning = true;
				transform.Rotation = Quaternion.Identity;
				float axisModifier = -Input.GetGamepadAxis(GamepadAxis.LeftX);
				Velocity.X = -1.0f;
				if (axisModifier != 0.0f)
					Velocity.X *= axisModifier;

				PlayRunningAnimation(delta);
			}
			else if (Input.IsKeyDown(KeyCode.D) || Input.GetGamepadAxis(GamepadAxis.LeftX) > ControllerDeadzone)
			{
				IsRunning = true;
				transform.Rotation = Quaternion.Identity;
				float axisModifier = Input.GetGamepadAxis(GamepadAxis.LeftX);
				Velocity.X = 1.0f;
				if (axisModifier != 0.0f)
					Velocity.X *= axisModifier;

					PlayRunningAnimation(delta);
			}
			else
				IsRunning = false;

			if (transform.Translation.Y >= FinishPoint)
				GameOver = true;

			if (GameOver && m_CameraEntity.DistanceToPlayer > 5.0f)
			{
				m_CameraEntity.DistanceToPlayer -= 1.0f * delta;

				if (m_CameraEntity.DistanceToPlayer <= 5.0f)
					SceneManager.LoadScene("Platformer");
			}

			if ((Input.IsKeyDown(KeyCode.Space) || Input.IsGamepadButtonDown(GamepadButton.Start)) && IsGrounded)
			{
				m_JumpSound.Play();
				Velocity.Y = 1.0f;
			}

			if (!IsGrounded)
			{
				m_Sprite.Texture = m_JumpTexture;
				Velocity.X /= HorizontalFriction;
			}
			else
				m_Sprite.Texture = m_NormalTexture;

			Velocity.X *= Speed * delta;
			Velocity.Y *= JumpForce * delta;
			m_Rigidbody.ApplyLinearImpulse(Velocity.XY, true);
		}

		private void PlayRunningAnimation(float delta)
		{
			if (IsGrounded)
				WaitTime -= delta;

			if (WaitTime <= 0.0f)
			{
				WaitTime = AnimationWaitTime;
				AnimationReady = true;
			}

			if (!IsGrounded && !AnimationReady)
				return;

			if (m_Sprite.Texture.Handle == m_NormalTexture.Handle)
			{
				m_Sprite.Texture = m_JumpTexture;
			}
			else
			{
				m_Sprite.Texture = m_NormalTexture;
			}

			AnimationReady = false;
		}

		protected override void OnDestroy()
		{
			Log.Info("Sandbox.PlatformerPlayer::OnDestroy()");
		}
	}

}
