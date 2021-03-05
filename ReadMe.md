# **Acoustic Diffusion Coefficient Simulation via Ray-Tracing**
*by Mateusz Janik*


```diff
!! Under Development !!
```
<p align="center">
<img src="githubContent/demo.gif" alt="animated" />
</p>


### TODO List:
- [x] change EnergyPerFrequency to unordered_map<float, energies>>
- [x] split simulation properties to at leas two bundles of class
- [x] fix constructor of the simulationProperties
- [x] add comments about energyPerFrequency, what it represent
- [x] add exception msg for all cases
- [ ] Include time factor inside simulation
- [ ] include ray expanding factor for lower frequencies
- [ ] rearrange trackers for cleaner code
- [ ] make a research how low frequencies interact with small obstacles
- [ ] use profile and search where is bottleneck
- [ ] explain what scene manager do
- [ ] consider changing name of the SceneManager
- [x] create dynamic reading object in gui
- [x] export energies to gui
- [x] create reference data
- [x] export reference data to gui
- [x] crate new tab inside of the gui to view results of the simulation
- [x] diffusion coefficient per frequency on gui (x must be log10)
- [ ] create polar pattern kon every frequency
 (create list to choose frequency per polar pattern)
- [x] calculate diffusion coefficient per frequency on gui
- [x] diffusor generator in Blender
- [ ] create RayTracer interface (hit with one ray get s reflected + random ones
could produce another rays)
- [ ] Normalized acoustic d coefficient graph
- [ ] loading screen for gui when simulation is running
- [ ] create gui for swaping models in current simulation (from reference to model and back to model)
- [ ] talk with promoter how to get more reliable data per frequency
- [ ] talk with promoter about reference data from mes
- [ ] analyze bottlenecks with profiler
- [ ] add multithreading support for speeding up experiments
- [ ] create architecture for running experiments
- [ ] create new ray-tracers to model wave phenomena