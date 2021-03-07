
async function WaitUntilSimulationIsReady() {
  while (loading) {
    await sleep(4000);
  }
  location.replace('./simulation.html');
}

WaitUntilSimulationIsReady();