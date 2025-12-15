// Highlight nav based on current page
window.addEventListener("DOMContentLoaded", function () {
  const navLinks = document.querySelectorAll("nav a");
  const current = window.location.pathname.split("/").pop();
  navLinks.forEach((link) => {
    const href = link.getAttribute("href");
    if ((current === "" && href === "index.html") || current === href) {
      link.classList.add("active");
    } else {
      link.classList.remove("active");
    }
  });

  // Fun Fact box (inline, changes on click or every 6 seconds)
  const facts = [
    "MiniWeb is written in C—one of the oldest modern programming languages.",
    "Did you know? The first web server was built by Tim Berners-Lee in 1990.",
    "Static websites are blazing fast because they don’t need a backend!",
    "HTML stands for HyperText Markup Language.",
    "CSS lets you color and style your websites without changing the HTML.",
  ];
  let factIndex = Math.floor(Math.random() * facts.length);

  function setFact() {
    const factBox = document.getElementById("fact-box");
    if (factBox) {
      factBox.textContent = facts[factIndex];
    }
  }
  function nextFact() {
    factIndex = (factIndex + 1) % facts.length;
    setFact();
  }
  // Cycle fact on click or timer
  const factContainer = document.getElementById("fact-container");
  if (factContainer) {
    setFact();
    factContainer.style.cursor = "pointer";
    factContainer.title = "Click for another fun fact!";
    factContainer.addEventListener("click", nextFact);

    setInterval(nextFact, 6000);
  }

  // Back to top button logic
  const box = document.querySelector(".content-box");
  const backTop = document.getElementById("back-to-top");
  if (box && backTop) {
    box.addEventListener("scroll", () => {
      if (box.scrollTop > 40) {
        backTop.style.display = "block";
      } else {
        backTop.style.display = "none";
      }
    });
    backTop.addEventListener("click", () => {
      box.scrollTo({ top: 0, behavior: "smooth" });
    });
  }
});
