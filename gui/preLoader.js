
const sleep = ms => new Promise(res => setTimeout(res, ms));

(async () => {
  while (loading) {
    await sleep(4000);
  }
  location.replace('./simulation.html');
})();
