#pragma once
#include <vector>
#include <algorithm>
#include <glm\glm.hpp>
#include <noise\noise.h>
#include <noise\interp.h>
#include <exception>
#include "..\Config.h"

namespace Antek
{
	using namespace noise;
	namespace WorldGeneration
	{
		class Biome
		{
		public:
			Biome(const double min_temperature, const double max_temperature)
			{
				this->_min_temperature = min_temperature;
				this->_max_temperature = max_temperature;
				if (max_temperature - min_temperature < BIOME_EDGE_FALLOFF + BIOME_EDGE_FALLOFF)
				{
					throw std::exception("Bad argument: the temp's range of the biome must be greater than the edgefalloff * 2");
				}
			}

			virtual uint8_t GetBlockId(int i, int j, int k, int height_point) = 0;

			double _min_temperature;
			double _max_temperature;
			noise::module::Module* _noise_module;
		};

		class MountainBiome : public Biome
		{
		public:
			MountainBiome(const int seed, const double min_temperature, const double max_temperature)
				: Biome(min_temperature, max_temperature)
			{
				const double MOUNTAIN_GLACIATION = 1.375;
				const double MOUNTAIN_LACUNARITY = 2.142578125;
				const double MOUNTAINS_TWIST = 0.0;
				const double MOUNTAINS_FREQUENCY = 0.01 / 1723.0;
				const double RIVERS_FREQUENCY = 1.0;
				const double RIVERS_DEEPNESS = 1 / 0.5;

				////////////////////////////////////////////////////////////////////////////
				// Module group: mountainous terrain
				////////////////////////////////////////////////////////////////////////////

				////////////////////////////////////////////////////////////////////////////
				// Module subgroup: mountain base definition (9 noise modules)
				//
				// This subgroup generates the base-mountain elevations.  Other subgroups
				// will add the ridges and low areas to the base elevations.
				//
				// -1.0 represents low mountainous terrain and +1.0 represents high
				// mountainous terrain.
				//

				// 1: [Mountain-ridge module]: This ridged-multifractal-noise module
				//    generates the mountain ridges.
				module::RidgedMulti* mountainBaseDef_rm0 = new module::RidgedMulti();
				mountainBaseDef_rm0->SetSeed(seed + 30);
				mountainBaseDef_rm0->SetFrequency(MOUNTAINS_FREQUENCY * 1723.0);
				mountainBaseDef_rm0->SetLacunarity(MOUNTAIN_LACUNARITY);
				mountainBaseDef_rm0->SetOctaveCount(4);
				mountainBaseDef_rm0->SetNoiseQuality(QUALITY_FAST);

				// 2: [Scaled-mountain-ridge module]: Next, a scale/bias module scales the
				//    output value from the mountain-ridge module so that its ridges are not
				//    too high.  The reason for this is that another subgroup adds actual
				//    mountainous terrain to these ridges.
				module::ScaleBias* mountainBaseDef_sb0 = new module::ScaleBias();
				mountainBaseDef_sb0->SetSourceModule(0, *mountainBaseDef_rm0);
				mountainBaseDef_sb0->SetScale(0.5);
				mountainBaseDef_sb0->SetBias(0.375);

				// 3: [River-valley module]: This ridged-multifractal-noise module generates
				//    the river valleys.  It has a much lower frequency than the mountain-
				//    ridge module so that more mountain ridges will appear outside of the
				//    valleys.  Note that this noise module generates ridged-multifractal
				//    noise using only one octave; this information will be important in the
				//    next step.
				module::RidgedMulti* mountainBaseDef_rm1 = new module::RidgedMulti();
				mountainBaseDef_rm1->SetSeed(seed + 31);
				mountainBaseDef_rm1->SetFrequency(MOUNTAINS_FREQUENCY * 367.0 * RIVERS_FREQUENCY);
				mountainBaseDef_rm1->SetLacunarity(MOUNTAIN_LACUNARITY);
				mountainBaseDef_rm1->SetOctaveCount(1);
				mountainBaseDef_rm1->SetNoiseQuality(QUALITY_BEST);

				module::Exponent* mountainBaseDef_ex = new module::Exponent();
				mountainBaseDef_ex->SetSourceModule(0, *mountainBaseDef_rm1);
				mountainBaseDef_ex->SetExponent(RIVERS_DEEPNESS);

				// 4: [Scaled-river-valley module]: Next, a scale/bias module applies a
				//    scaling factor of -2.0 to the output value from the river-valley
				//    module.  This stretches the possible elevation values because one-
				//    octave ridged-multifractal noise has a lower range of output values
				//    than multiple-octave ridged-multifractal noise.  The negative scaling
				//    factor inverts the range of the output value, turning the ridges from
				//    the river-valley module into valleys.
				module::ScaleBias* mountainBaseDef_sb1 = new module::ScaleBias();
				mountainBaseDef_sb1->SetSourceModule(0, *mountainBaseDef_ex);
				mountainBaseDef_sb1->SetScale(-2.0);
				mountainBaseDef_sb1->SetBias(-1.1);

				// 5: [Low-flat module]: This low constant value is used by step 6.
				module::Const* mountainBaseDef_co = new module::Const();
				mountainBaseDef_co->SetConstValue(-1.0);

				// 6: [Mountains-and-valleys module]: This blender module merges the
				//    scaled-mountain-ridge module and the scaled-river-valley module
				//    together.  It causes the low-lying areas of the terrain to become
				//    smooth, and causes the high-lying areas of the terrain to contain
				//    ridges.  To do this, it uses the scaled-river-valley module as the
				//    control module, causing the low-flat module to appear in the lower
				//    areas and causing the scaled-mountain-ridge module to appear in the
				//    higher areas.
				module::Blend* mountainBaseDef_bl = new module::Blend();
				mountainBaseDef_bl->SetSourceModule(0, *mountainBaseDef_co);
				mountainBaseDef_bl->SetSourceModule(1, *mountainBaseDef_sb0);
				mountainBaseDef_bl->SetControlModule(*mountainBaseDef_sb1);

				// 9: [Mountain-base-definition subgroup]: Caches the output value from the
				//    warped-mountains-and-valleys module.
				module::Cache* mountainBaseDef = new module::Cache();
				mountainBaseDef->SetSourceModule(0, *mountainBaseDef_bl);

				// 5: [Scaled-mountainous-terrain-module]: This scale/bias module slightly
				//    reduces the range of the output value from the combined-mountainous-
				//    terrain module, decreasing the heights of the mountain peaks.
				module::ScaleBias* mountainousTerrain_sb2 = new module::ScaleBias();
				mountainousTerrain_sb2->SetSourceModule(0, *mountainBaseDef);
				mountainousTerrain_sb2->SetScale(0.8);
				mountainousTerrain_sb2->SetBias(0.5);

				// 6: [Glaciated-mountainous-terrain-module]: This exponential-curve module
				//    applies an exponential curve to the output value from the scaled-
				//    mountainous-terrain module.  This causes the slope of the mountains to
				//    smoothly increase towards higher elevations, as if a glacier grinded
				//    out those mountains.  This exponential-curve module expects the output
				//    value to range from -1.0 to +1.0.
				module::Exponent* mountainousTerrain_ex = new module::Exponent();
				mountainousTerrain_ex->SetSourceModule(0, *mountainousTerrain_sb2);
				mountainousTerrain_ex->SetExponent(MOUNTAIN_GLACIATION);

				module::ScaleBias* mountainsTerrainScaled = new module::ScaleBias();
				mountainsTerrainScaled->SetSourceModule(0, *mountainousTerrain_ex);
				mountainsTerrainScaled->SetScale(0.7);
				mountainsTerrainScaled->SetBias(0.2);

				// 7: [Mountainous-terrain group]: Caches the output value from the
				//    glaciated-mountainous-terrain module.  This is the output value for
				//    the entire mountainous-terrain group.
				module::Cache* mountainousTerrain = new module::Cache();
				mountainousTerrain->SetSourceModule(0, *mountainsTerrainScaled);

				this->_noise_module = mountainousTerrain;
			}

			virtual uint8_t GetBlockId(int i, int j, int k, int height_point)
			{
				if (j == height_point)
					return 1;
				else
					return 3;
			}
		};

		class FlatBiome : public Biome
		{
		public:
			FlatBiome(const int seed, const double min_temperature, const double max_temperature)
				: Biome(min_temperature, max_temperature)
			{
				module::Billow* baseFlatTerrain = new module::Billow();
				baseFlatTerrain->SetFrequency(0.004);
				baseFlatTerrain->SetSeed(seed + 20);
				baseFlatTerrain->SetNoiseQuality(QUALITY_STD);
				module::ScaleBias* flatTerrain = new module::ScaleBias();
				flatTerrain->SetSourceModule(0, *baseFlatTerrain);
				flatTerrain->SetScale(0.08);
				flatTerrain->SetBias(0.15);

				this->_noise_module = flatTerrain;
			}

			virtual uint8_t GetBlockId(int i, int j, int k, int height_point)
			{
				return 1;
			}
		};

		class DesetBiome : public Biome
		{
		public:
			DesetBiome(const int seed, const double min_temperature, const double max_temperature)
				: Biome(min_temperature, max_temperature)
			{
				const double DUNES_FREQUENCY = 4.0;
				const double DUNES_AMPLITUDE = 0.3;
				const double CLIFFS_FREQUENCY = 10.0;

				module::RidgedMulti* badlandsSand_rm = new module::RidgedMulti();
				badlandsSand_rm->SetSeed(seed + 80);
				badlandsSand_rm->SetFrequency(0.0018 * DUNES_FREQUENCY);
				badlandsSand_rm->SetLacunarity(2.212890625);
				badlandsSand_rm->SetNoiseQuality(QUALITY_STD);
				badlandsSand_rm->SetOctaveCount(1);

				// 2: [Scaled-sand-dunes module]: This scale/bias module shrinks the dune
				//    heights by a small amount.  This is necessary so that the subsequent
				//    noise modules in this subgroup can add some detail to the dunes.
				module::ScaleBias* badlandsSand_sb0 = new module::ScaleBias();
				badlandsSand_sb0->SetSourceModule(0, *badlandsSand_rm);
				badlandsSand_sb0->SetScale(0.875);
				badlandsSand_sb0->SetBias(0.1);

				// 3: [Dune-detail module]: This noise module uses Voronoi polygons to
				//    generate the detail to add to the dunes.  By enabling the distance
				//    algorithm, small polygonal pits are generated; the edges of the pits
				//    are joined to the edges of nearby pits.
				module::Voronoi* badlandsSand_vo = new module::Voronoi();
				badlandsSand_vo->SetSeed(seed + 81);
				badlandsSand_vo->SetFrequency(0.01 * DUNES_FREQUENCY);
				badlandsSand_vo->SetDisplacement(0.0);
				badlandsSand_vo->EnableDistance();

				// 4: [Scaled-dune-detail module]: This scale/bias module shrinks the dune
				//    details by a large amount.  This is necessary so that the subsequent
				//    noise modules in this subgroup can add this detail to the sand-dunes
				//    module.
				module::ScaleBias* badlandsSand_sb1 = new module::ScaleBias();
				badlandsSand_sb1->SetSourceModule(0, *badlandsSand_vo);
				badlandsSand_sb1->SetScale(0.15);
				badlandsSand_sb1->SetBias(0.3);

				// 5: [Dunes-with-detail module]: This addition module combines the scaled-
				//    sand-dunes module with the scaled-dune-detail module.
				module::Add* badlandsSand_ad = new module::Add();
				badlandsSand_ad->SetSourceModule(0, *badlandsSand_sb0);
				badlandsSand_ad->SetSourceModule(1, *badlandsSand_sb1);

				// 6: [Badlands-sand subgroup]: Caches the output value from the dunes-with-
				//    detail module.
				module::Cache* badlandsSand = new module::Cache();
				badlandsSand->SetSourceModule(0, *badlandsSand_sb0 /* TODO: THIS IS THE VORONOI PART badlandsSand_ad */);

				////////////////////////////////////////////////////////////////////////////
				// Module subgroup: badlands cliffs (7 noise modules)
				//
				// This subgroup generates the cliffs for the badlands.
				//
				// -1.0 represents the lowest elevations and +1.0 represents the highest
				// elevations.
				//

				// 1: [Cliff-basis module]: This Perlin-noise module generates some coherent
				//    noise that will be used to generate the cliffs.
				module::Perlin* badlandsCliffs_pe = new module::Perlin();
				badlandsCliffs_pe->SetSeed(seed + 90);
				badlandsCliffs_pe->SetFrequency(0.000524375 * CLIFFS_FREQUENCY);
				badlandsCliffs_pe->SetPersistence(0.5);
				badlandsCliffs_pe->SetLacunarity(2.212890625);
				badlandsCliffs_pe->SetOctaveCount(6);
				badlandsCliffs_pe->SetNoiseQuality(QUALITY_STD);

				// 2: [Cliff-shaping module]: Next, this curve module applies a curve to the
				//    output value from the cliff-basis module.  This curve is initially
				//    very shallow, but then its slope increases sharply.  At the highest
				//    elevations, the curve becomes very flat again.  This produces the
				//    stereotypical Utah-style desert cliffs.
				module::Curve* badlandsCliffs_cu = new module::Curve();
				badlandsCliffs_cu->SetSourceModule(0, *badlandsCliffs_pe);
				badlandsCliffs_cu->AddControlPoint(-2.0000, -2.0000);
				badlandsCliffs_cu->AddControlPoint(-1.0000, -1.2500);
				badlandsCliffs_cu->AddControlPoint(-0.0000, -0.7500);
				badlandsCliffs_cu->AddControlPoint(0.5000, -0.2500);
				badlandsCliffs_cu->AddControlPoint(0.6250, 0.8750);
				badlandsCliffs_cu->AddControlPoint(0.7500, 1.0000);
				badlandsCliffs_cu->AddControlPoint(2.0000, 1.2500);

				// 3: [Clamped-cliffs module]: This clamping module makes the tops of the
				//    cliffs very flat by clamping the output value from the cliff-shaping
				//    module so that the tops of the cliffs are very flat.
				module::Clamp* badlandsCliffs_cl = new module::Clamp();
				badlandsCliffs_cl->SetSourceModule(0, *badlandsCliffs_pe);
				badlandsCliffs_cl->SetBounds(0, 0.875);

				// 4: [Terraced-cliffs module]: Next, this terracing module applies some
				//    terraces to the clamped-cliffs module in the lower elevations before
				//    the sharp cliff transition.
				module::Terrace* badlandsCliffs_te = new module::Terrace();
				badlandsCliffs_te->SetSourceModule(0, *badlandsCliffs_cl);
				badlandsCliffs_te->AddControlPoint(-1.0000);
				badlandsCliffs_te->AddControlPoint(-0.8750);
				badlandsCliffs_te->AddControlPoint(-0.7500);
				badlandsCliffs_te->AddControlPoint(-0.5000);
				badlandsCliffs_te->AddControlPoint(0.0000);
				badlandsCliffs_te->AddControlPoint(1.0000);


				// 7: [Badlands-cliffs subgroup]: Caches the output value from the warped-
				//    cliffs module.
				module::Cache* badlandsCliffs = new module::Cache();
				badlandsCliffs->SetSourceModule(0, *badlandsCliffs_te);


				////////////////////////////////////////////////////////////////////////////
				// Module subgroup: badlands terrain (3 noise modules)
				//
				// Generates the final badlands terrain.
				//
				// Using a scale/bias module, the badlands sand is flattened considerably,
				// then the sand elevations are lowered to around -1.0.  The maximum value
				// from the flattened sand module and the cliff module contributes to the
				// final elevation.  This causes sand to appear at the low elevations since
				// the sand is slightly higher than the cliff base.
				//
				// -1.0 represents the lowest elevations and +1.0 represents the highest
				// elevations.
				//

				// 1: [Scaled-sand-dunes module]: This scale/bias module considerably
				//    flattens the output value from the badlands-sands subgroup and lowers
				//    this value to near -1.0.
				module::ScaleBias* badlandsTerrain_sb = new module::ScaleBias();
				badlandsTerrain_sb->SetSourceModule(0, *badlandsSand);
				badlandsTerrain_sb->SetScale(DUNES_AMPLITUDE);
				badlandsTerrain_sb->SetBias(0.3);

				// 2: [Dunes-and-cliffs module]: This maximum-value module causes the dunes
				//    to appear in the low areas and the cliffs to appear in the high areas.
				//    It does this by selecting the maximum of the output values from the
				//    scaled-sand-dunes module and the badlands-cliffs subgroup.
				module::Max* badlandsTerrain_ma = new module::Max();
				badlandsTerrain_ma->SetSourceModule(0, *badlandsCliffs);
				badlandsTerrain_ma->SetSourceModule(1, *badlandsTerrain_sb);

				// 3: [Badlands-terrain group]: Caches the output value from the dunes-and-
				//    cliffs module.  This is the output value for the entire badlands-
				//    terrain group.
				module::Cache* badlandsTerrain = new module::Cache();
				badlandsTerrain->SetSourceModule(0, *badlandsTerrain_ma);

				this->_noise_module = badlandsTerrain;
			}

			virtual uint8_t GetBlockId(int i, int j, int k, int height_point)
			{
				return 2;
			}
		};
	}
}