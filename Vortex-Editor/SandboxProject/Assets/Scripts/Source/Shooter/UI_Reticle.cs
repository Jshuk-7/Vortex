﻿using Vortex;

namespace Sandbox.Shooter {

	using Shooter.Weapons;

	public class UI_Reticle : Entity
	{
		public Vector4 color;
		public Vector2 size;
		WeaponManager weaponManager;

		protected override void OnCreate()
		{
			weaponManager = Scene.FindEntityByName("Weapon Manager").As<WeaponManager>();
		}

		protected override void OnUpdate(float deltaTime)
		{
			DrawReticle();
		}

		void DrawReticle()
		{
			if (weaponManager.GetCurrentWeapon() == WeaponType.None)
				return;

			bool rightMouseButtonPressed = Input.IsMouseButtonDown(MouseButton.Right);
			bool leftTriggerPressed = Input.GetGamepadAxis(GamepadAxis.LeftTrigger) > 0f;

			if (!rightMouseButtonPressed && !leftTriggerPressed)
			{
				DebugRenderer.BeginScene();
				DebugRenderer.DrawQuadBillboard(transform.Translation + new Vector3(0.01f, 0.01f, 0), size, color);
				DebugRenderer.Flush();
			}
		}
	}

}
