﻿using System;
using System.Runtime.InteropServices;

namespace Vortex
{
	[StructLayout(LayoutKind.Sequential)]
	public struct Quaternion : IEquatable<Quaternion>
	{
		public float W;
		public float X;
		public float Y;
		public float Z;

		public static Quaternion Identity => new Quaternion(1, 0, 0, 0);

		public Quaternion(float w, float x, float y, float z)
		{
			W = w;
			X = x;
			Y = y;
			Z = z;
		}

		public Quaternion(Vector3 xyz, float w)
		{
			W = w;
			X = xyz.X;
			Y = xyz.Y;
			Z = xyz.Z;
		}

		public Quaternion(Vector3 euler)
		{
			Vector3 c = Vector3.Cos(euler * 0.5f);
			Vector3 s = Vector3.Sin(euler * 0.5f);

			W = c.X * c.Y * c.Z + s.X * s.Y * s.Z;
			X = s.X * c.Y * c.Z - c.X * s.Y * s.Z;
			Y = c.X * s.Y * c.Z + s.X * c.Y * s.Z;
			Z = c.X * c.Y * s.Z - s.X * s.Y * c.Z;
		}

		public Quaternion(float x, float y, float z)
		{
			Vector3 euler = new Vector3(x, y, z);
			Vector3 c = Vector3.Cos(euler * 0.5f);
			Vector3 s = Vector3.Sin(euler * 0.5f);

			W = c.X * c.Y * c.Z + s.X * s.Y * s.Z;
			X = s.X * c.Y * c.Z - c.X * s.Y * s.Z;
			Y = c.X * s.Y * c.Z + s.X * c.Y * s.Z;
			Z = c.X * c.Y * s.Z - s.X * s.Y * c.Z;
		}

		public static Vector3 operator *(Quaternion q, Vector3 v)
		{
			Vector3 qv = new Vector3(q.X, q.Y, q.Z);
			Vector3 uv = Vector3.Cross(qv, v);
			Vector3 uuv = Vector3.Cross(qv, uv);
			return v + ((uv * q.W) + uuv) * 2.0f;
		}

		public static Quaternion operator *(Quaternion a, Quaternion b)
		{
			Quaternion result = new Quaternion();
			result.W = a.W * b.W - a.X * b.X - a.Y * b.Y - a.Z * b.Z;
			result.X = a.W * b.X + a.X * b.W + a.Y * b.Z - a.Z * b.Y;
			result.Y = a.W * b.Y + a.Y * b.W + a.Z * b.X - a.X * b.Z;
			result.Z = a.W * b.Z + a.Z * b.W + a.X * b.Y - a.Y * b.X;
			return result;
		}

		public Vector3 XYZ
		{
			get => new Vector3(X, Y, Z);

			set
			{
				X = value.X;
				Y = value.Y;
				Z = value.Z;
			}
		}

		public override int GetHashCode() => (W, X, Y, Z).GetHashCode();
		public override bool Equals(object obj) => obj is Quaternion other && Equals(other);
		public bool Equals(Quaternion right) => W == right.W && X == right.X && Y == right.Y && Z == right.Z;

		public static bool operator ==(Quaternion left, Quaternion right) => left.Equals(right);
		public static bool operator !=(Quaternion left, Quaternion right) => !(left == right);

		public float Length => Mathf.Sqrt(LengthSquared);

		public float LengthSquared => X * X + Y * Y + Z * Z + W * W;

		public static Quaternion FromToRotation(Vector3 aFrom, Vector3 aTo)
		{
			Vector3 axis = Vector3.Cross(aFrom, aTo);
			float angle = Vector3.Angle(aFrom, aTo);
			return AngleAxis(angle, axis.Normalized());
		}

		public static Quaternion AngleAxis(float aAngle, Vector3 aAxis)
		{
			aAxis.Normalize();
			float rad = (Mathf.Deg2Rad * aAngle) * 0.5f;
			aAxis *= Mathf.Sin(rad);
			return new Quaternion(Mathf.Cos(rad), aAxis.X, aAxis.Y, aAxis.Z);
		}

		public static Quaternion LookRotation(Vector3 forward) => LookRotation(forward, Vector3.Up);

		public static Quaternion LookRotation(Vector3 forward, Vector3 up)
		{
			forward.Normalize();

			Vector3 vector = forward.Normalized();
			Vector3 vector2 = Vector3.Cross(up, vector).Normalized();
			Vector3 vector3 = Vector3.Cross(vector, vector2);
			var m00 = vector2.X;
			var m01 = vector2.Y;
			var m02 = vector2.Z;
			var m10 = vector3.X;
			var m11 = vector3.Y;
			var m12 = vector3.Z;
			var m20 = vector.X;
			var m21 = vector.Y;
			var m22 = vector.Z;

			float num8 = (m00 + m11) + m22;
			var quaternion = new Quaternion();
			if (num8 > 0f)
			{
				var num = Mathf.Sqrt(num8 + 1f);
				quaternion.W = num * 0.5f;
				num = 0.5f / num;
				quaternion.X = (m12 - m21) * num;
				quaternion.Y = (m20 - m02) * num;
				quaternion.Z = (m01 - m10) * num;
				return quaternion;
			}
			if ((m00 >= m11) && (m00 >= m22))
			{
				var num7 = Mathf.Sqrt(((1f + m00) - m11) - m22);
				var num4 = 0.5f / num7;
				quaternion.X = 0.5f * num7;
				quaternion.Y = (m01 + m10) * num4;
				quaternion.Z = (m02 + m20) * num4;
				quaternion.W = (m12 - m21) * num4;
				return quaternion;
			}
			if (m11 > m22)
			{
				var num6 = Mathf.Sqrt(((1f + m11) - m00) - m22);
				var num3 = 0.5f / num6;
				quaternion.X = (m10 + m01) * num3;
				quaternion.Y = 0.5f * num6;
				quaternion.Z = (m21 + m12) * num3;
				quaternion.W = (m20 - m02) * num3;
				return quaternion;
			}
			var num5 = Mathf.Sqrt(((1f + m22) - m00) - m11);
			var num2 = 0.5f / num5;
			quaternion.X = (m20 + m02) * num2;
			quaternion.Y = (m21 + m12) * num2;
			quaternion.Z = 0.5f * num5;
			quaternion.W = (m01 - m10) * num2;
			return quaternion;
		}

		public void Normalize()
		{
			float scale = 1.0f / Length;
			XYZ *= scale;
			W *= scale;
		}

		public Vector3 Eulers
		{
			get => Mathf.EulerAngles(this);
		}

		public static Quaternion EulerAngles(Vector3 eulers) => new Quaternion(eulers);
		public static Quaternion EulerAngles(float x, float y, float z) => new Quaternion(new Vector3(x, y, z));

		public static Quaternion Slerp(Quaternion a, Quaternion b, float t)
		{
			if (t > 1) t = 1;
			if (t < 0) t = 0;
			return SlerpUnclamped(a, b, t);
		}

		public static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t)
		{
			// if either input is zero, return the other.
			if (a.LengthSquared == 0.0f)
			{
				if (b.LengthSquared == 0.0f)
				{
					return new Quaternion(1, 0, 0, 0);
				}
				return b;
			}
			else if (b.LengthSquared == 0.0f)
			{
				return a;
			}

			float cosHalfAngle = a.W * b.W + Vector3.Dot(a.XYZ, b.XYZ);

			if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
			{
				// angle = 0.0f, so just return one input.
				return a;
			}
			else if (cosHalfAngle < 0.0f)
			{
				b.XYZ = -b.XYZ;
				b.W = -b.W;
				cosHalfAngle = -cosHalfAngle;
			}

			float blendA;
			float blendB;
			if (cosHalfAngle < 0.99f)
			{
				// do proper slerp for big angles
				float halfAngle = Mathf.Acos(cosHalfAngle);
				float sinHalfAngle = Mathf.Sin(halfAngle);
				float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
				blendA = Mathf.Sin(halfAngle * (1.0f - t)) * oneOverSinHalfAngle;
				blendB = Mathf.Sin(halfAngle * t) * oneOverSinHalfAngle;
			}
			else
			{
				// do lerp if angle is really small.
				blendA = 1.0f - t;
				blendB = t;
			}

			Quaternion result = new Quaternion(blendA * a.XYZ + blendB * b.XYZ, blendA * a.W + blendB * b.W);
			if (result.LengthSquared > 0.0f)
			{
				result.Normalize();
				return result;
			}

			return Identity;
		}

		public static Quaternion Inverse(Quaternion quat)
		{
			InternalCalls.Quaternion_Inverse(ref quat, out Quaternion result);
			return result;
		}

	}
}
