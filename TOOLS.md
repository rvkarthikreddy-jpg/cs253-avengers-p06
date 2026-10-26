# TOOLS.md: Declaration of Tools and AI Assistance

**Project:** Project 06 — Percolation with Union-Find  
**Team:** Avengers  
**Course:** CS 253 / MA 253 Data Structures and Algorithms Laboratory  
**Institute:** IIT Indore  

---

## 1. Tool Declaration

### Gemini (Google Large Language Model)
* **Model / Version:** Gemini (Web / Conversational Interface)
* **Purpose:** 
  * Formatting Markdown tables and scaffolding template headers.
  * Formatting LaTeX equations and document syntax for team onboarding instructions and analyst directives.
  * Checking standard C++ `#ifndef` header guard naming conventions.
* **Representative Prompts:**
  * *"Draft the formal structural invariants for disjoint set union in Markdown format."*
  * *"Provide a clean standalone LaTeX template for team instructions."*
  * *"Formulate the standard 2D percolation finite-size scaling equation in Markdown LaTeX."*
* **Files / Modules Containing Tool-Assisted Content:**
  * `include/disjoint_set.hpp`: Header guard syntax and Doxygen comment formatting.
  * `INTERFACE.md`: Formatting of Markdown math tables and invariant equations.
  * `REPORT.md`: Section outline structure and LaTeX formatting of Section 9 equations.
  * `docs/`: LaTeX syntax for onboarding and directive guides.
* **Human Verification:**
  * All method signatures, invariants, models, and code logic were reviewed, checked, and approved by the respective team members before committing.

---

## 2. Other Third-Party Libraries
* No external data structure or graph libraries (such as Boost or pre-built disjoint-set packages) were used. All data structures and algorithms are implemented from scratch.
