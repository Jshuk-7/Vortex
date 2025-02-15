﻿using System;
using Vortex;

namespace Sandbox {

	public class HelpMenu : Entity
	{
		protected override void OnUpdate(float delta)
		{
			if (Input.IsGamepadButtonDown(GamepadButton.A))
				SceneManager.LoadScene("Platformer2");
		}

		protected override void OnGui()
		{
			Gui.Begin("Tutorial", Application.GetPosition());

			Gui.Text("Welcome!");
			Gui.Spacing();
			Gui.Text("Move Around with the Left Stick");
			Gui.Text("Press A to Jump");
			Gui.Text("Press Y to load the next Scene");

			Gui.End();
		}
	}

}
