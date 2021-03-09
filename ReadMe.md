# **Acoustic Diffusion Coefficient Simulation via Ray-Tracing**
*by Mateusz Janik*


```diff
!! Under Development !!
```
<p align="center">
<img src="githubContent/demo.gif" alt="animated" />
</p>


### TODO List:
- [x] Normalized acoustic d coefficient graph
- [x] loading  screen for gui when simulation is running
- [x] rearrange trackers for cleaner code
- [ ] create polar pattern on every frequency
- [ ] create gui for swaping models in current simulation (from reference to model and back to model)
- [ ] Include time factor inside simulation
- [ ] include ray expanding factor for lower frequencies
- [x] make a research how low frequencies interact with small obstacles
- [ ] use profile and search where is bottleneck
- [x] explain what scene manager do
 (create list to choose frequency per polar pattern)
- [ ] create RayTracer interface (hit with one ray get s reflected + random ones
could produce another rays)
- [ ] talk with promoter how to get more reliable data per frequency
- [ ] talk with promoter about reference data from mes
- [ ] analyze bottlenecks with profiler
- [ ] add multithreading support for speeding up experiments
- [ ] create architecture for running experiments
- [ ] create new ray-tracers to model wave phenomena