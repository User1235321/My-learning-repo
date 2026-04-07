#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>

struct Production
{
  char left;
  std::string right;
  int id;
};

bool isTerminal(char c)
{
  return c == '+' || c == '*' || c == 'i' || c == '(' || c == ')' || c == '$';
}

bool isNonTerminal(char c)
{
  return c == 'S' || c == 'E' || c == 'A' || c == 'T' || c == 'B' || c == 'F';
}

std::map< char, std::set< char > > computeFirst(
    const std::vector< Production > & prods,
    const std::set< char > & nonterms)
{
  std::map< char, std::set< char > > first;
  for (char nt : nonterms) first[nt] = std::set< char >();
  bool changed = true;
  while (changed)
  {
    changed = false;
    for (const auto & p : prods)
    {
      char A = p.left;
      std::string rhs = p.right;
      std::set< char > current;
      bool allNullable = true;
      size_t i = 0;
      while (i < rhs.size())
      {
        char sym = rhs[i];
        if (isTerminal(sym))
        {
          current.insert(sym);
          allNullable = false;
          break;
        }
        else
        {
          const auto & firstB = first[sym];
          for (char t : firstB)
            if (t != 0) current.insert(t);
          if (firstB.find(0) == firstB.end())
          {
            allNullable = false;
            break;
          }
          ++i;
        }
      }
      if (allNullable) current.insert(0);
      for (char t : current)
      {
        if (first[A].find(t) == first[A].end())
        {
          first[A].insert(t);
          changed = true;
        }
      }
    }
  }
  return first;
}

std::map< char, std::set< char > > computeFollow(
    const std::vector< Production > & prods,
    const std::set< char > & nonterms,
    const std::map< char, std::set< char > > & first,
    const std::map< char, bool > & nullable)
{
  std::map< char, std::set< char > > follow;
  for (char nt : nonterms) follow[nt] = std::set< char >();
  follow['S'].insert('$');
  bool changed = true;
  while (changed)
  {
    changed = false;
    for (const auto & p : prods)
    {
      char A = p.left;
      std::string rhs = p.right;
      for (size_t i = 0; i < rhs.size(); ++i)
      {
        char B = rhs[i];
        if (isNonTerminal(B))
        {
          std::set< char > firstBeta;
          bool allNullableBeta = true;
          size_t j = i + 1;
          while (j < rhs.size())
          {
            char sym = rhs[j];
            if (isTerminal(sym))
            {
              firstBeta.insert(sym);
              allNullableBeta = false;
              break;
            }
            else
            {
              const auto & firstB = first.at(sym);
              for (char t : firstB)
                if (t != 0) firstBeta.insert(t);
              if (firstB.find(0) == firstB.end())
              {
                allNullableBeta = false;
                break;
              }
              ++j;
            }
          }
          for (char t : firstBeta)
          {
            if (follow[B].find(t) == follow[B].end())
            {
              follow[B].insert(t);
              changed = true;
            }
          }
          if (allNullableBeta)
          {
            for (char t : follow[A])
            {
              if (follow[B].find(t) == follow[B].end())
              {
                follow[B].insert(t);
                changed = true;
              }
            }
          }
        }
      }
    }
  }
  return follow;
}

bool derivesEpsilon(const std::string & alpha,
                    const std::map< char, bool > & nullable)
{
  for (char c : alpha)
  {
    if (isTerminal(c)) return false;
    if (!nullable.at(c)) return false;
  }
  return true;
}

std::map< int, std::set< char > > computeLookahead(
    const std::vector< Production > & prods,
    const std::map< char, std::set< char > > & first,
    const std::map< char, std::set< char > > & follow,
    const std::map< char, bool > & nullable)
{
  std::map< int, std::set< char > > la;
  for (const auto & p : prods)
  {
    std::string rhs = p.right;
    std::set< char > firstRhs;
    size_t i = 0;
    bool allNullable = true;
    while (i < rhs.size())
    {
      char sym = rhs[i];
      if (isTerminal(sym))
      {
        firstRhs.insert(sym);
        allNullable = false;
        break;
      }
      else
      {
        const auto & firstB = first.at(sym);
        for (char t : firstB)
          if (t != 0) firstRhs.insert(t);
        if (firstB.find(0) == firstB.end())
        {
          allNullable = false;
          break;
        }
        ++i;
      }
    }
    if (allNullable)
    {
      la[p.id] = firstRhs;
      for (char t : follow.at(p.left))
        la[p.id].insert(t);
    }
    else
    {
      la[p.id] = firstRhs;
    }
  }
  return la;
}

struct Node
{
  char symbol;
  int step;
  std::vector< Node * > children;
  Node(char s, int st) : symbol(s), step(st) {}
};

void printTree(Node * node, int depth, std::ostream & out)
{
  for (int i = 0; i < depth; ++i) out << "  ";
  if (node->symbol == 0)
    out << "ε";
  else
    out << node->symbol;
  if (node->step > 0) out << " " << node->step;
  out << "\n";
  for (Node * child : node->children)
    printTree(child, depth + 1, out);
}

void deleteTree(Node * node)
{
  for (Node * child : node->children)
    deleteTree(child);
  delete node;
}

int main()
{
  std::vector< Production > prods =
  {
    {'S', "E$", 0},
    {'E', "TA", 1},
    {'A', "+TA", 2},
    {'A', "", 3},
    {'T', "FB", 4},
    {'B', "*FB", 5},
    {'B', "", 6},
    {'F', "i", 7},
    {'F', "(E)", 8}
  };

  std::set< char > terminals = {'+', '*', 'i', '(', ')', '$'};
  std::set< char > nonterminals = {'S', 'E', 'A', 'T', 'B', 'F'};

  std::map< char, bool > nullable;
  for (char nt : nonterminals) nullable[nt] = false;
  bool changed = true;
  while (changed)
  {
    changed = false;
    for (const auto & p : prods)
    {
      if (p.right.empty())
      {
        if (!nullable[p.left])
        {
          nullable[p.left] = true;
          changed = true;
        }
        continue;
      }
      bool allNull = true;
      for (char c : p.right)
      {
        if (isTerminal(c)) { allNull = false; break; }
        if (!nullable[c]) { allNull = false; break; }
      }
      if (allNull && !nullable[p.left])
      {
        nullable[p.left] = true;
        changed = true;
      }
    }
  }

  auto first = computeFirst(prods, nonterminals);
  auto follow = computeFollow(prods, nonterminals, first, nullable);
  auto lookahead = computeLookahead(prods, first, follow, nullable);

  std::map< char, std::map< char, int > > table;
  for (char nt : nonterminals)
    for (char t : terminals) table[nt][t] = -1;

  bool ll1 = true;
  for (const auto & p : prods)
  {
    char A = p.left;
    for (char t : lookahead[p.id])
    {
      if (t == 0) continue;
      if (table[A][t] != -1)
      {
        std::cout << "Conflict for " << A << " on " << t
                  << ": rules " << table[A][t] << " and " << p.id << "\n";
        ll1 = false;
        break;
      }
      table[A][t] = p.id;
    }
    if (!ll1) break;
  }

  if (!ll1)
  {
    std::cout << "Grammar is not LL(1).\n";
    return 1;
  }

  std::cout << "Grammar is LL(1). LOOKAHEAD table:\n\n";
  std::cout << "    | + | * | i | ( | ) | $\n";
  std::cout << "----+---+---+---+---+---+---\n";
  for (char nt : {'S', 'E', 'A', 'T', 'B', 'F'})
  {
    std::cout << " " << nt << "  |";
    for (char t : {'+', '*', 'i', '(', ')', '$'})
    {
      int rid = table[nt][t];
      if (rid == -1)
        std::cout << "   |";
      else
        std::cout << " " << rid << " |";
    }
    std::cout << "\n";
  }

  std::cout << "\nEnter word to parse (must end with $): ";
  std::string inputWord;
  std::cin >> inputWord;

  std::cout << "\nParsing: " << inputWord << "\n\n";

  std::stack< std::pair< char, Node* > > st;
  Node * root = new Node('S', 0);
  st.push({'S', root});
  std::string remaining = inputWord;
  int step = 1;
  std::vector< int > appliedRules;
  std::vector< Node* > allNodes;
  allNodes.push_back(root);

  std::cout << "Parser trace:\n";
  while (!(st.empty() && remaining.empty()))
  {
    std::string stackStr;
    std::stack< std::pair< char, Node* > > temp = st;
    std::vector< char > rev;
    while (!temp.empty())
    {
      rev.push_back(temp.top().first);
      temp.pop();
    }
    for (auto it = rev.rbegin(); it != rev.rend(); ++it)
      stackStr.push_back(*it);
    std::cout << "(" << step << "):" << stackStr << "," << remaining << "\n";

    if (st.empty())
    {
      std::cout << "Error: empty stack, input not empty\n";
      break;
    }

    char topSym = st.top().first;
    Node * topNode = st.top().second;
    char nextInput = remaining.empty() ? 0 : remaining[0];

    if (isTerminal(topSym))
    {
      if (topSym == nextInput)
      {
        st.pop();
        remaining = remaining.substr(1);
        topNode->step = step;
      }
      else
      {
        std::cout << "Error: terminal " << topSym
                  << " != input " << nextInput << "\n";
        break;
      }
    }
    else
    {
      int ruleId = table[topSym][nextInput];
      if (ruleId == -1)
      {
        std::cout << "Error: no rule for " << topSym
                  << " on input " << nextInput << "\n";
        break;
      }
      appliedRules.push_back(ruleId);
      const Production & p = prods[ruleId];
      st.pop();
      std::string rhs = p.right;
      topNode->step = step;
      for (int i = rhs.size() - 1; i >= 0; --i)
      {
        char sym = rhs[i];
        Node * child = new Node(sym, 0);
        allNodes.push_back(child);
        topNode->children.insert(topNode->children.begin(), child);
        st.push({sym, child});
      }
      if (rhs.empty())
      {
        Node * eps = new Node(0, step);
        allNodes.push_back(eps);
        topNode->children.push_back(eps);
      }
    }
    ++step;
  }

  if (st.empty() && remaining.empty())
  {
    std::cout << "(" << step << "):" << "," << "\n";
    std::cout << "Stack empty at end of input ===> Word accepted.\n";
  }

  std::cout << "\nDerivation steps (rules applied):\n";
  int cnt = 0;
  for (int rid : appliedRules)
  {
    const Production & p = prods[rid];
    std::cout << "  (" << ++cnt << ") " << p.left << " -> "
              << (p.right.empty() ? "ε" : p.right) << "\n";
  }

  std::cout << "\nParse tree:\n";
  printTree(root, 0, std::cout);

  deleteTree(root);
  return 0;
}
