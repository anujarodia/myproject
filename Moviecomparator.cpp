#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <thread>
#include <limits>
#include <algorithm>
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
using namespace std;
struct movie {
string title;
string director;
string year;
vector<string> actors;
vector<int> ratings;
bool awarded;
// Constructor with default arguments
movie(string t = "", string d = "", string y = "", vector<string> a = {} , vector<int> r = {} , bool aw =
false)
: title(move(t)), director(move(d)), year(y), actors(move(a)) , ratings(move(r)) , awarded(move(aw)) {}
};
void clearInputBuffer() {
cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
struct graph {
unordered_map<string, movie> moviedetails;
unordered_map<string, vector<pair<string, int>>> adjlist;
void addvertex(const movie& movie) {
if (moviedetails.find(movie.title) == moviedetails.end()) {
adjlist[movie.title] = {};
moviedetails[movie.title] = movie;
}
}
void addedge(string u, string v, int weight) {
if (adjlist.find(u) == adjlist.end() || adjlist.find(v) == adjlist.end()) {
cerr << "One or both movies not found.\n";
cout << u << " and " << v;
return;
}
adjlist[u].push_back({v, weight});
adjlist[v].push_back({u, weight});
}
bool movieexists(const string& movietitle) {
return moviedetails.find(movietitle) != moviedetails.end();
}
void printawardedmovies() {
vector<string> awardedmovies;
for (const auto& movie : moviedetails) {
if (movie.second.awarded) {
awardedmovies.push_back(movie.second.title);
}
}
if (!awardedmovies.empty()) {
cout << "Awarded movies:\n";
for (const auto& movie : awardedmovies) {
cout << movie << endl;
}
} else {
cout << "No awarded movies found.\n";
}
}
void findtopratedmovies(int genre) {
vector<pair<string, int>> movieratings;
for (const auto& movie : moviedetails) {
movieratings.push_back({movie.second.title, movie.second.ratings[genre]});
}
sort(movieratings.begin(), movieratings.end(), [](const auto& a, const auto& b) {
return a.second > b.second;
});
cout << "Top 10 movies in ";
if (genre == 0) cout << "action genre are : \n";
else if (genre == 1) cout << "comedy genre are : \n";
else if (genre == 2) cout << "thriller genre are : \n";
else if (genre == 3) cout << "romance genre are : \n";
for (int i = 0; i < min(10, static_cast<int>(movieratings.size())); ++i) {
cout << i + 1 << ". " << movieratings[i].first << " - Rating: " << movieratings[i].second << endl;
}
}
void findmoviewithsameactors(const string& movietitle) {
if (moviedetails.find(movietitle) == moviedetails.end()) {
cout << "Movie not found in the database.\n";
return;
}
const movie& movietocompare = moviedetails[movietitle];
bool found = false;
cout << "Movies with the same actors as " << movietitle << ":\n";
for (const auto& movie : moviedetails) {
if (movie.second.title != movietocompare.title) {
const vector<string>& actors1 = movie.second.actors;
const vector<string>& actors2 = movietocompare.actors;
// Check if there's at least one common actor
for (const string& actor : actors1) {
if (find(actors2.begin(), actors2.end(), actor) != actors2.end()) {
cout << "- " << movie.second.title << endl;
found = true;
break; // Found a common actor, move to the next movie
}
}
}
}
if (!found) {
cout << "No movie found with the same actors as " << movietitle << endl;
}
}
void findclosestratedmovies(const string& movietitle) {
if (moviedetails.find(movietitle) == moviedetails.end()) {
cout << "Movie not found in the database.\n";
return;
}
const movie& movietocompare = moviedetails[movietitle];
int minratingdifference = INT_MAX;
vector<string> closestmovies;
for (const auto& movie : moviedetails) {
if (movie.second.title != movietocompare.title) {
const vector<int>& ratings1 = movie.second.ratings;
const vector<int>& ratings2 = movietocompare.ratings;
// Calculate the absolute difference between ratings
int ratingdifference = 0;
for (size_t i = 0; i < ratings1.size(); ++i) {
ratingdifference += abs(ratings1[i] - ratings2[i]);
}
if (ratingdifference < minratingdifference) {
minratingdifference = ratingdifference;
closestmovies.clear();
closestmovies.push_back(movie.second.title);
} else if (ratingdifference == minratingdifference) {
closestmovies.push_back(movie.second.title);
}
}
}
if (!closestmovies.empty()) {
cout << "Closest rated movie(s) to " << movietitle << ":\n";
for (const auto& movie : closestmovies) {
cout << "- " << movie << endl;
}
} else {
cout << "No movie found with similar ratings to " << movietitle << endl;
}
}
void findmoviebysamedirector(const string& movietitle) {
if (moviedetails.find(movietitle) == moviedetails.end()) {
cout << "Movie not found in the database.\n";
return;
}
const movie& movietocompare = moviedetails[movietitle];
for (const auto& movie : moviedetails) {
if (movie.second.title != movietocompare.title &&
movie.second.director == movietocompare.director) {
cout << "Movie with the same director as " << movietitle << ": " << movie.second.title << endl;
return;
}
}
cout << "No movie found by the same director as " << movietitle << endl;
}
// function to find a movie by the same release year as a given movie title
void findmoviebysameyear(const string& movietitle) {
if (moviedetails.find(movietitle) == moviedetails.end()) {
cout << "Movie not found in the database.\n";
return;
}
const movie& movietocompare = moviedetails[movietitle];
for (const auto& movie : moviedetails) {
if (movie.second.title != movietocompare.title &&
movie.second.year == movietocompare.year) {
cout << "Movie with the same year as " << movietitle << ": " << movie.second.title << endl;
return;
}
}
cout << "No movie found with the same year as " << movietitle << endl;
}
void findmoviesbyactor(const string& actorname) {
vector<string> moviesbyactor;
for (const auto& movie : moviedetails) {
if (find(movie.second.actors.begin(), movie.second.actors.end(), actorname) !=
movie.second.actors.end()) {
moviesbyactor.push_back(movie.second.title);
}
}
if (!moviesbyactor.empty()) {
cout << "Movies featuring actor " << actorname << ":\n";
for (const auto& movie : moviesbyactor) {
cout << movie << endl;
}
} else {
cout << "No movies found featuring actor " << actorname << endl;
}
}
void findmoviesbydirector(const string& directorname) {
vector<string> moviesbydirector;
for (const auto& movie : moviedetails) {
if (movie.second.director == directorname) {
moviesbydirector.push_back(movie.second.title);
}
}
if (!moviesbydirector.empty()) {
cout << "Movies directed by " << directorname << ":\n";
for (const auto& movie : moviesbydirector) {
cout << movie << endl;
}
} else {
cout << "No movies found directed by " << directorname << endl;
}
}
void findmoviesbyyear(const string& targetyear) {
vector<string> moviesbyyear;
for (const auto& movie : moviedetails) {
if (movie.second.year == targetyear) {
moviesbyyear.push_back(movie.second.title);
}
}
if (!moviesbyyear.empty()) {
cout << "Movies released in " << targetyear << ":\n";
for (const auto& movie : moviesbyyear) {
cout << movie << endl;
}
} else {
cout << "No movies found released in " << targetyear << endl;
}
}
void dijkstra(const string& startmovie) {
if (!movieexists(startmovie)) {
cout << RED << "Sorry we dont have this movie in our database " << RESET << endl;
} else {
priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
unordered_map<string, int> distance;
for (const auto& node : adjlist) {
distance[node.first] = numeric_limits<int>::max();
}
distance[startmovie] = 0;
pq.push({ 0, startmovie });
while (!pq.empty()) {
string current = pq.top().second;
int curdist = pq.top().first;
pq.pop();
for (const auto& neighbor : adjlist[current]) {
int newdist = curdist + neighbor.second;
if (newdist < distance[neighbor.first]) {
distance[neighbor.first] = newdist;
pq.push({ newdist, neighbor.first });
}
}
}
// find the nearest movie(s) from the calculated distances
vector<string> nearestmovies;
int mindistance = numeric_limits<int>::max();
for (const auto& dist : distance) {
if (dist.second < mindistance && dist.first != startmovie) {
nearestmovies.clear();
nearestmovies.push_back(dist.first);
mindistance = dist.second;
} else if (dist.second == mindistance && dist.first != startmovie) {
nearestmovies.push_back(dist.first);
}
}
if (!nearestmovies.empty()) {
cout << "Nearest movie(s) to " << startmovie << " at distance " << mindistance << " is/are:" << endl;
for (const auto& movie : nearestmovies) {
cout << movie << endl;
cout << "Details of the nearest movie:" << endl;
cout << "Title: " << moviedetails[movie].title << endl;
cout << "Director: " << moviedetails[movie].director << endl;
cout << "Year: " << moviedetails[movie].year << endl;
cout << "Actors: ";
for (const auto& actor : moviedetails[movie].actors) {
cout << actor << ", ";
}
cout << "\n-----------------------\n";
}
} else {
cout << "No nearest movie found for " << startmovie << endl;
}
}
}
void displaymoviedetails() {
for (const auto& entry : moviedetails) {
cout << "Title: " << entry.second.title << endl;
cout << "Director: " << entry.second.director << endl;
cout << "Year: " << entry.second.year << endl;
cout << "Actors: ";
for (const auto& actor : entry.second.actors) {
cout << actor << ", ";
}
cout << "\n-----------------------\n";
}
}
void displaygraph() {
for (const auto& entry : adjlist) {
cout << entry.first << ": ";
for (const auto& neighbor : entry.second) {
cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
}
cout << endl;
}
}
};
void loader(int time){
for (int i = 0; i < 10; ++i) {
cout << ".";
cout.flush();
this_thread::sleep_for(chrono::milliseconds(time));
}
cout<<"\n\n";
}
int main() {
graph actiongraph;
graph romancegraph;
graph comedygraph;
graph thrillergraph;
movie movie1("The Shawshank Redemption", "Frank Darabont", "1994", {"Tim Robbins", "Morgan
Freeman"}, {4, 2, 8, 1} , true);
movie movie2("Top Gun", "Tony Scott", "1986", {"Tom Cruise", "Kelly McGillis"}, {9, 3, 6, 7});
movie movie3("Django Unchained", "Quentin Tarantino", "2012", {"Jamie Foxx", "Christoph Waltz"},
{8, 5, 7, 2});
movie movie4("The Dark Knight", "Christopher Nolan", "2008", {"Christian Bale", "Heath Ledger"}, {9,
3, 8, 2} , true);
movie movie5("Inception", "Christopher Nolan", "2010", {"Leonardo DiCaprio", "Joseph GordonLevitt"}, {9, 2, 7, 4} , true);
movie movie6("Interstellar", "Christopher Nolan", "2014", {"Matthew McConaughey", "Anne
Hathaway"}, {8, 2, 8, 3} , true);
movie movie7("Due Date", "Todd Phillips", "2010", {"Robert Downey Jr.", "Zach Galifianakis"}, {6, 7,
5, 4});
movie movie8("21 Jump Street", "Phil Lord, Christopher Miller", "2012", {"Jonah Hill", "Channing
Tatum"}, {8, 7, 5, 3});
movie movie9("The Dictator", "Larry Charles", "2012", {"Sacha Baron Cohen", "Anna Faris"}, {7, 8, 3,
2});
movie movie10("Superbad", "Greg Mottola", "2007", {"Michael Cera", "Jonah Hill"}, {5, 8, 4, 2});
movie movie11("The Nice Guys", "Shane Black", "2016", {"Russell Crowe", "Ryan Gosling"}, {8, 7, 6,
3});
movie movie12("Zodiac", "David Fincher", "2007", {"Jake Gyllenhaal", "Mark Ruffalo"}, {6, 3, 8, 2} ,
true);
movie movie13("Shutter Island", "Martin Scorsese", "2010", {"Leonardo DiCaprio", "Mark Ruffalo"},
{7, 2, 9, 4});
movie movie14("Seven", "David Fincher", "1995", {"Morgan Freeman", "Brad Pitt"}, {8, 3, 8, 1});
movie movie15("John Wick", "Chad Stahelski", "2014", {"Keanu Reeves", "Michael Nyqvist"}, {9, 3, 7,
1});
movie movie16("American Psycho", "Mary Harron", "2000", {"Christian Bale", "Justin Theroux"}, {7,
2, 8, 2});
movie movie17("The Hangover", "Todd Phillips", "2009", {"Bradley Cooper", "Ed Helms"}, {6, 9, 5, 2});
movie movie18("Fast X", "Justin Lin", "2023", {"Vin Diesel", "Paul Walker"}, {9, 4, 7, 2});
movie movie19("Prisoners", "Denis Villeneuve", "2013", {"Hugh Jackman", "Jake Gyllenhaal"}, {8, 2,
9, 2});
movie movie20("The Prestige", "Christopher Nolan", "2006", {"Christian Bale", "Hugh Jackman"}, {7,
3, 8, 3} , true);
movie movie21("Deadpool", "Tim Miller", "2016", {"Ryan Reynolds", "Morena Baccarin"}, {9, 8, 8,
1});
movie movie22("Don't Look Up", "Adam McKay", "2021", {"Leonardo DiCaprio", "Jennifer
Lawrence"}, {7, 6, 6, 3});
movie movie23("Gone Girl", "David Fincher", "2014", {"Ben Affleck", "Rosamund Pike"}, {7, 3, 8, 4} ,
true);
movie movie24("Drive", "Nicolas Winding Refn", "2011", {"Ryan Gosling", "Carey Mulligan"}, {8, 2, 7,
5});
movie movie25("Iron Man", "Jon Favreau", "2008", {"Robert Downey Jr.", "Gwyneth Paltrow"}, {9, 4,
6, 3});
movie movie26("The Avengers", "Joss Whedon", "2012", {"Robert Downey Jr.", "Chris Evans"}, {9, 3,
6, 2});
movie movie27("Transformers", "Michael Bay", "2007", {"Shia LaBeouf", "Megan Fox"}, {8, 3, 6, 2} ,
true);
movie movie28("Fight Club", "David Fincher", "1999", {"Brad Pitt", "Edward Norton"}, {9, 3, 9, 2} ,
true);
movie movie29("The Adjustment Bureau", "George Nolfi", "2011", {"Matt Damon", "Emily Blunt"},
{7, 4, 6, 5});
movie movie30("Home Alone", "Chris Columbus", "1990", {"Macaulay Culkin", "Joe Pesci"}, {5, 7, 4,
1});
movie movie31("La La Land", "Damien Chazelle", "2016", {"Ryan Gosling", "Emma Stone"}, {4, 6, 5, 9}
, true);
movie movie32("Bullet Train", "David Leitch", "2022", {"Brad Pitt", "Joey King"}, {8, 2, 6, 1});
movie movie33("Justice League", "Zack Snyder", "2017", {"Ben Affleck", "Gal Gadot"}, {8, 3, 6, 2});
movie movie34("Palm Springs", "Max Barbakow", "2020", {"Andy Samberg", "Cristin Milioti"}, {6, 7,
5, 3});
movie movie35("Blade Runner", "Ridley Scott", "1982", {"Harrison Ford", "Rutger Hauer"}, {9, 2, 8, 2}
, true);
movie movie36("Rush Hour", "Brett Ratner", "1998", {"Jackie Chan", "Chris Tucker"}, {8, 7, 5, 1} ,
true);
movie movie37("Free Guy", "Shawn Levy", "2021", {"Ryan Reynolds", "Jodie Comer"}, {8, 6, 5, 3});
movie movie38("Ted", "Seth MacFarlane", "2012", {"Mark Wahlberg", "Mila Kunis"}, {6, 9, 3, 2});
actiongraph.addvertex(movie1);
actiongraph.addvertex(movie2);
actiongraph.addvertex(movie3);
actiongraph.addvertex(movie4);
actiongraph.addvertex(movie5);
actiongraph.addvertex(movie6);
actiongraph.addvertex(movie7);
actiongraph.addvertex(movie8);
actiongraph.addvertex(movie9);
actiongraph.addvertex(movie10);
actiongraph.addvertex(movie11);
actiongraph.addvertex(movie12);
actiongraph.addvertex(movie13);
actiongraph.addvertex(movie14);
actiongraph.addvertex(movie15);
actiongraph.addvertex(movie16);
actiongraph.addvertex(movie17);
actiongraph.addvertex(movie18);
actiongraph.addvertex(movie19);
actiongraph.addvertex(movie20);
actiongraph.addvertex(movie21);
actiongraph.addvertex(movie22);
actiongraph.addvertex(movie23);
actiongraph.addvertex(movie24);
actiongraph.addvertex(movie25);
actiongraph.addvertex(movie26);
actiongraph.addvertex(movie27);
actiongraph.addvertex(movie28);
actiongraph.addvertex(movie29);
actiongraph.addvertex(movie30);
actiongraph.addvertex(movie31);
actiongraph.addvertex(movie32);
actiongraph.addvertex(movie33);
actiongraph.addvertex(movie34);
actiongraph.addvertex(movie35);
actiongraph.addvertex(movie36);
actiongraph.addvertex(movie37);
actiongraph.addvertex(movie38);
romancegraph.addvertex(movie1);
romancegraph.addvertex(movie2);
romancegraph.addvertex(movie3);
romancegraph.addvertex(movie4);
romancegraph.addvertex(movie5);
romancegraph.addvertex(movie6);
romancegraph.addvertex(movie7);
romancegraph.addvertex(movie8);
romancegraph.addvertex(movie9);
romancegraph.addvertex(movie10);
romancegraph.addvertex(movie11);
romancegraph.addvertex(movie12);
romancegraph.addvertex(movie13);
romancegraph.addvertex(movie14);
romancegraph.addvertex(movie15);
romancegraph.addvertex(movie16);
romancegraph.addvertex(movie17);
romancegraph.addvertex(movie18);
romancegraph.addvertex(movie19);
romancegraph.addvertex(movie20);
romancegraph.addvertex(movie21);
romancegraph.addvertex(movie22);
romancegraph.addvertex(movie23);
romancegraph.addvertex(movie24);
romancegraph.addvertex(movie25);
romancegraph.addvertex(movie26);
romancegraph.addvertex(movie27);
romancegraph.addvertex(movie28);
romancegraph.addvertex(movie29);
romancegraph.addvertex(movie30);
romancegraph.addvertex(movie31);
romancegraph.addvertex(movie32);
romancegraph.addvertex(movie33);
romancegraph.addvertex(movie34);
romancegraph.addvertex(movie35);
romancegraph.addvertex(movie36);
romancegraph.addvertex(movie37);
romancegraph.addvertex(movie38);
thrillergraph.addvertex(movie1);
thrillergraph.addvertex(movie2);
thrillergraph.addvertex(movie3);
thrillergraph.addvertex(movie4);
thrillergraph.addvertex(movie5);
thrillergraph.addvertex(movie6);
thrillergraph.addvertex(movie7);
thrillergraph.addvertex(movie8);
thrillergraph.addvertex(movie9);
thrillergraph.addvertex(movie10);
thrillergraph.addvertex(movie11);
thrillergraph.addvertex(movie12);
thrillergraph.addvertex(movie13);
thrillergraph.addvertex(movie14);
thrillergraph.addvertex(movie15);
thrillergraph.addvertex(movie16);
thrillergraph.addvertex(movie17);
thrillergraph.addvertex(movie18);
thrillergraph.addvertex(movie19);
thrillergraph.addvertex(movie20);
thrillergraph.addvertex(movie21);
thrillergraph.addvertex(movie22);
thrillergraph.addvertex(movie23);
thrillergraph.addvertex(movie24);
thrillergraph.addvertex(movie25);
thrillergraph.addvertex(movie26);
thrillergraph.addvertex(movie27);
thrillergraph.addvertex(movie28);
thrillergraph.addvertex(movie29);
thrillergraph.addvertex(movie30);
thrillergraph.addvertex(movie31);
thrillergraph.addvertex(movie32);
thrillergraph.addvertex(movie33);
thrillergraph.addvertex(movie34);
thrillergraph.addvertex(movie35);
thrillergraph.addvertex(movie36);
thrillergraph.addvertex(movie37);
thrillergraph.addvertex(movie38);
actiongraph.addedge("The Shawshank Redemption", "Zodiac", 5);
actiongraph.addedge("The Shawshank Redemption", "Palm Springs", 4);
actiongraph.addedge("The Shawshank Redemption", "The Nice Guys", 6);
actiongraph.addedge("Zodiac", "Shutter Island", 4);
actiongraph.addedge("Seven", "Shutter Island", 2);
actiongraph.addedge("Seven", "John Wick", 4);
actiongraph.addedge("Seven", "American Psycho", 3);
actiongraph.addedge("The Nice Guys","Superbad" , 5);
actiongraph.addedge("21 Jump Street","Superbad" , 2);
actiongraph.addedge("21 Jump Street","The Dictator" , 3);
actiongraph.addedge("Due Date","The Dictator" , 2);
actiongraph.addedge("Due Date","Interstellar" , 8);
actiongraph.addedge("Inception","Interstellar" , 2);
actiongraph.addedge("Inception","Django Unchained" , 4);
actiongraph.addedge("Top Gun","Django Unchained" , 8);
actiongraph.addedge("Inception","Top Gun" , 3);
actiongraph.addedge("Inception","The Dark Knight" , 5);
actiongraph.addedge("Top Gun","The Dark Knight" , 2);
actiongraph.addedge("Due Date","Ted" , 1);
actiongraph.addedge("Free Guy","Ted" , 3);
actiongraph.addedge("Free Guy","Rush Hour" , 1);
actiongraph.addedge("Blade Runner","Rush Hour" , 3);
actiongraph.addedge("Blade Runner","Justice League" , 2);
actiongraph.addedge("Blade Runner","Palm Springs" , 6);
actiongraph.addedge("Bullet Train","Justice League" , 3);
actiongraph.addedge("Bullet Train","Home Alone" , 6);
actiongraph.addedge("La La Land","Home Alone" , 2);
actiongraph.addedge("La La Land","Fight Club" , 9);
actiongraph.addedge("Transformers","Fight Club" , 5);
actiongraph.addedge("Transformers","The Avengers" , 2);
actiongraph.addedge("Drive","The Avengers" , 7);
actiongraph.addedge("Iron Man","The Avengers" , 1);
actiongraph.addedge("Iron Man","Deadpool" , 2);
actiongraph.addedge("Don't Look Up","Deadpool" , 9);
actiongraph.addedge("The Prestige","Deadpool" , 8);
actiongraph.addedge("The Prestige","Prisoners" , 2);
actiongraph.addedge("The Prestige","Gone Girl" , 3);
actiongraph.addedge("Drive","Gone Girl" , 4);
actiongraph.addedge("Prisoners","Fast X" , 3);
actiongraph.addedge("The Hangover","Fast X" , 8);
actiongraph.addedge("The Hangover","Don't Look Up" , 4);
actiongraph.addedge("American Psycho","Fast X" , 4);
actiongraph.addedge("Transformers","The Adjustment Bureau" , 6);
actiongraph.addedge("Rush Hour","The Adjustment Bureau" , 2);
actiongraph.addedge("Justice League","The Adjustment Bureau" , 7);
comedygraph.addvertex(movie1);
comedygraph.addvertex(movie2);
comedygraph.addvertex(movie3);
comedygraph.addvertex(movie4);
comedygraph.addvertex(movie5);
comedygraph.addvertex(movie6);
comedygraph.addvertex(movie7);
comedygraph.addvertex(movie8);
comedygraph.addvertex(movie9);
comedygraph.addvertex(movie10);
comedygraph.addvertex(movie11);
comedygraph.addvertex(movie12);
comedygraph.addvertex(movie13);
comedygraph.addvertex(movie14);
comedygraph.addvertex(movie15);
comedygraph.addvertex(movie16);
comedygraph.addvertex(movie17);
comedygraph.addvertex(movie18);
comedygraph.addvertex(movie19);
comedygraph.addvertex(movie20);
comedygraph.addvertex(movie21);
comedygraph.addvertex(movie22);
comedygraph.addvertex(movie23);
comedygraph.addvertex(movie24);
comedygraph.addvertex(movie25);
comedygraph.addvertex(movie26);
comedygraph.addvertex(movie27);
comedygraph.addvertex(movie28);
comedygraph.addvertex(movie29);
comedygraph.addvertex(movie30);
comedygraph.addvertex(movie31);
comedygraph.addvertex(movie32);
comedygraph.addvertex(movie33);
comedygraph.addvertex(movie34);
comedygraph.addvertex(movie35);
comedygraph.addvertex(movie36);
comedygraph.addvertex(movie37);
comedygraph.addvertex(movie38);
comedygraph.addedge("The Shawshank Redemption", "Zodiac", 2);
comedygraph.addedge("The Shawshank Redemption", "Palm Springs", 7);
comedygraph.addedge("The Shawshank Redemption", "The Nice Guys", 7);
comedygraph.addedge("Zodiac", "Shutter Island", 3);
comedygraph.addedge("Seven", "Shutter Island", 2);
comedygraph.addedge("Seven", "John Wick", 4);
comedygraph.addedge("Seven", "American Psycho", 3);
comedygraph.addedge("The Nice Guys","Superbad" , 2);
comedygraph.addedge("21 Jump Street","Superbad" , 1);
comedygraph.addedge("21 Jump Street","The Dictator" , 2);
comedygraph.addedge("Due Date","The Dictator" , 3);
comedygraph.addedge("Due Date","Interstellar" , 8);
comedygraph.addedge("Inception","Interstellar" , 3);
comedygraph.addedge("Inception","Django Unchained" , 5);
comedygraph.addedge("Top Gun","Django Unchained" , 6);
comedygraph.addedge("Inception","Top Gun" , 6);
comedygraph.addedge("Inception","The Dark Knight" , 4);
comedygraph.addedge("Top Gun","The Dark Knight" , 4);
comedygraph.addedge("Due Date","Ted" ,1 );
comedygraph.addedge("Free Guy","Ted" , 1);
comedygraph.addedge("Free Guy","Rush Hour" , 2);
comedygraph.addedge("Blade Runner","Rush Hour" , 8);
comedygraph.addedge("Blade Runner","Justice League" , 3);
comedygraph.addedge("Blade Runner","Palm Springs" , 7);
comedygraph.addedge("Bullet Train","Justice League" , 7);
comedygraph.addedge("Bullet Train","Home Alone" , 1);
comedygraph.addedge("La La Land","Home Alone" , 2);
comedygraph.addedge("La La Land","Fight Club" , 7);
comedygraph.addedge("Transformers","Fight Club" , 4);
comedygraph.addedge("Transformers","The Avengers" , 3);
comedygraph.addedge("Drive","The Avengers" , 7);
comedygraph.addedge("Iron Man","The Avengers" , 1);
comedygraph.addedge("Iron Man","Deadpool" , 2);
comedygraph.addedge("Don't Look Up","Deadpool" , 3);
comedygraph.addedge("The Prestige","Deadpool" , 9);
comedygraph.addedge("The Prestige","Prisoners" , 3);
comedygraph.addedge("The Prestige","Gone Girl" , 6);
comedygraph.addedge("Drive","Gone Girl" , 3);
comedygraph.addedge("Prisoners","Fast X" , 7);
comedygraph.addedge("The Hangover","Fast X" , 4);
comedygraph.addedge("The Hangover","Don't Look Up" , 2);
comedygraph.addedge("American Psycho","Fast X" , 6);
comedygraph.addedge("Transformers","The Adjustment Bureau" , 5);
comedygraph.addedge("Rush Hour","The Adjustment Bureau" ,7 );
comedygraph.addedge("Justice League","The Adjustment Bureau" , 6);
romancegraph.addedge("The Shawshank Redemption", "Zodiac", 3);
romancegraph.addedge("The Shawshank Redemption", "Palm Springs", 6);
romancegraph.addedge("The Shawshank Redemption", "The Nice Guys", 6);
romancegraph.addedge("Zodiac", "Shutter Island", 7);
romancegraph.addedge("Seven", "Shutter Island", 4);
romancegraph.addedge("Seven", "John Wick", 8);
romancegraph.addedge("Seven", "American Psycho", 3);
romancegraph.addedge("The Nice Guys","Superbad" , 4);
romancegraph.addedge("21 Jump Street","Superbad" , 3);
romancegraph.addedge("21 Jump Street","The Dictator" , 4);
romancegraph.addedge("Due Date","The Dictator" , 3);
romancegraph.addedge("Due Date","Interstellar" , 6);
romancegraph.addedge("Inception","Interstellar" , 7);
romancegraph.addedge("Inception","Django Unchained" , 6);
romancegraph.addedge("Top Gun","Django Unchained" , 8);
romancegraph.addedge("Inception","Top Gun" , 4);
romancegraph.addedge("Inception","The Dark Knight" , 5);
romancegraph.addedge("Top Gun","The Dark Knight" , 3);
romancegraph.addedge("Due Date","Ted" , 1);
romancegraph.addedge("Free Guy","Ted" , 2);
romancegraph.addedge("Free Guy","Rush Hour" , 3);
romancegraph.addedge("Blade Runner","Rush Hour" , 8);
romancegraph.addedge("Blade Runner","Justice League" , 7);
romancegraph.addedge("Blade Runner","Palm Springs" , 3);
romancegraph.addedge("Bullet Train","Justice League" , 3);
romancegraph.addedge("Bullet Train","Home Alone" , 7);
romancegraph.addedge("La La Land","Home Alone" , 5);
romancegraph.addedge("La La Land","Fight Club" , 3);
romancegraph.addedge("La La Land","Palm Springs" , 1);
romancegraph.addedge("La La Land","Top Gun" , 2);
romancegraph.addedge("Transformers","Fight Club" , 2);
romancegraph.addedge("Transformers","The Avengers" , 4);
romancegraph.addedge("Drive","The Avengers" , 4);
romancegraph.addedge("Iron Man","The Avengers" , 5);
romancegraph.addedge("Iron Man","Deadpool" , 3);
romancegraph.addedge("Don't Look Up","Deadpool" , 1);
romancegraph.addedge("The Prestige","Deadpool" , 7);
romancegraph.addedge("The Prestige","Prisoners" , 4);
romancegraph.addedge("The Prestige","Gone Girl" , 7);
romancegraph.addedge("Drive","Gone Girl" , 3);
romancegraph.addedge("Prisoners","Fast X" , 8);
romancegraph.addedge("The Hangover","Fast X" , 2);
romancegraph.addedge("The Hangover","Don't Look Up" , 3);
romancegraph.addedge("American Psycho","Fast X" , 7);
romancegraph.addedge("Transformers","The Adjustment Bureau" , 6);
romancegraph.addedge("Rush Hour","The Adjustment Bureau" , 3);
romancegraph.addedge("Justice League","The Adjustment Bureau" , 5);
thrillergraph.addedge("The Shawshank Redemption", "Zodiac", 4);
thrillergraph.addedge("The Shawshank Redemption", "Palm Springs", 8);
thrillergraph.addedge("The Shawshank Redemption", "The Nice Guys", 3);
thrillergraph.addedge("Zodiac", "Shutter Island", 1);
thrillergraph.addedge("Seven", "Shutter Island", 2);
thrillergraph.addedge("Seven", "John Wick", 3);
thrillergraph.addedge("Seven", "American Psycho", 2);
thrillergraph.addedge("The Nice Guys","Superbad" , 7);
thrillergraph.addedge("21 Jump Street","Superbad" , 3);
thrillergraph.addedge("21 Jump Street","The Dictator" , 4);
thrillergraph.addedge("Due Date","The Dictator" , 3);
thrillergraph.addedge("Due Date","Interstellar" , 8);
thrillergraph.addedge("Inception","Interstellar" , 2);
thrillergraph.addedge("Inception","Django Unchained" , 3);
thrillergraph.addedge("Top Gun","Django Unchained" , 4);
thrillergraph.addedge("Inception","Top Gun" , 3);
thrillergraph.addedge("Inception","The Dark Knight" , 1);
thrillergraph.addedge("Top Gun","The Dark Knight" , 2);
thrillergraph.addedge("Due Date","Ted" ,5 );
thrillergraph.addedge("Free Guy","Ted" , 4);
thrillergraph.addedge("Free Guy","Rush Hour" , 3);
thrillergraph.addedge("Blade Runner","Rush Hour" , 3);
thrillergraph.addedge("Blade Runner","Justice League" , 4);
thrillergraph.addedge("Blade Runner","Palm Springs" , 6);
thrillergraph.addedge("Bullet Train","Justice League" , 2);
thrillergraph.addedge("Bullet Train","Home Alone" , 8);
thrillergraph.addedge("La La Land","Home Alone" , 4);
thrillergraph.addedge("La La Land","Fight Club" , 9);
thrillergraph.addedge("Transformers","Fight Club" , 3);
thrillergraph.addedge("Transformers","The Avengers" , 2);
thrillergraph.addedge("Drive","The Avengers" , 4);
thrillergraph.addedge("Iron Man","The Avengers" , 3);
thrillergraph.addedge("Iron Man","Deadpool" , 4);
thrillergraph.addedge("Don't Look Up","Deadpool" , 3);
thrillergraph.addedge("The Prestige","Deadpool" , 7);
thrillergraph.addedge("The Prestige","Prisoners" , 2);
thrillergraph.addedge("The Prestige","Gone Girl" , 3);
thrillergraph.addedge("Drive","Gone Girl" , 6);
thrillergraph.addedge("Prisoners","Fast X" , 3);
thrillergraph.addedge("The Hangover","Fast X" , 8);
thrillergraph.addedge("The Hangover","Don't Look Up" , 7);
thrillergraph.addedge("American Psycho","Fast X" , 3);
thrillergraph.addedge("Transformers","The Adjustment Bureau" , 4);
thrillergraph.addedge("Rush Hour","The Adjustment Bureau" ,6 );
thrillergraph.addedge("Justice League","The Adjustment Bureau" , 7);
cout<<"\n\n---------------------------------------------------------------------------------------------------------------------
--------"<<endl;
cout<<"\n\n"<<GREEN;
cout << " $$$$$$\\ $$\\ $$\\ $$\\ " << endl;
cout << " $$ __$$\\ $$$\\ $$$ | \\__| " << endl;
cout << " $$ / \\__| $$$$$$\\ $$$\\$\\ $$$$ | $$$$$$\\ $$\\ $$\\ $$\\ $$$$$$\\
$$$$$$$\\ " << endl;
cout << " $$ |$$$$\\ $$ __$$\\ $$\\$$ $$ $$ |$$ __$$\\$$\\ $$ |$$ |$$ __$$\\ $$
_____|" << endl;
cout << " $$ |\\_$$ |$$ / $$ | $$ \\$$$ $$ |$$ / $$ |\\$$\\$$ / $$ |$$$$$$$$
|\\$$$$$$\\ " << endl;
cout << " $$ | $$ |$$ | $$ | $$ |\\$ /$$ |$$ | $$ | \\$$$ / $$ |$$ ____|
\\____$$\\ " << endl;
cout << " \\$$$$$$ |\\$$$$$$ | $$ | \\_/ $$ |\\$$$$$$ | \\$ / $$ |\\$$$$$$$\\
$$$$$$$ |" << endl;
cout << " \\______/ \\______/ \\__| \\__| \\______/ \\_/ \\__|
\\_______|\\_______/ " << endl;
cout << endl;
cout<<RESET<<endl;
cout<<"---------------------------------------------------------------------------------------------------------------------------
-\n"<<endl;
cout<<YELLOW<<" Welcome to Go Movies: Your Gateway to Unforgettable Movie
Adventures! "<<RESET<<endl;
cout<<" Get a movie recommended based on your preferences :-\n";
cout<<CYAN<<" We suggest you the best movies based on preferences of movies that
you give us !!\n"<<RESET;
while (1){
cout<<"\n\n Enter your favourite movie's name -> ";
string name;
getline(cin , name);
cout<<GREEN;
loader(300);
cout<<RESET<<"\033[33m"<<"\nHere are your movies----\n\n"<<RESET;
cout<<CYAN;
cout<<"Recommended movie in aspect of action : \n"<<endl;
actiongraph.dijkstra(name);
loader(200);
cout<<RESET<<GREEN"Recommended movie in aspect of comedy : \n"<<endl;
comedygraph.dijkstra(name);
loader(200);
cout<<RESET<<RED"Recommended movie in aspect of romance : \n"<<endl;
romancegraph.dijkstra(name);
loader(200);
cout<<RESET<<YELLOW<<"Recommended movie in aspect of thriller : \n"<<endl;
thrillergraph.dijkstra(name);
cout<<RESET;
int choice;
cout<<"Press 1 for more filters 2 to search again\n->";
cin>>choice;
if(choice == 1){
cout<<"Here are some filters to search for your desired movie\n";
cout<<"1) Top Rated movies by genre\n2) Awarded and critically aclaimed movies\n3) Movie search
by actor \n4) Movie search by director\n5) Movie search by Year of release\n-> ";
cin>>choice;
clearInputBuffer();
int c2;
switch (choice)
{
case 1:
int genre;
cout<<"Select the genre \n1) Action \n2) Comedy \n3) Thriller\n4) Romance\n->";
cin>>genre;
clearInputBuffer();
if(genre == 1 || genre ==2 || genre ==3 || genre ==4){
actiongraph.findtopratedmovies(genre-1);
}
else{
cout<<"\nWrong choice entred";
}
break;
case 2:
actiongraph.printawardedmovies();
break;
case 3:
cout<<"\n1) Search movie of same actor by movie name 2) Seach movie by actor \n-> ";
cin>>c2;
if(c2 == 2){
cout<<"Enter actor name -> ";
string moviename;
clearInputBuffer();
getline(cin , moviename);
//cout<<"You entered actoer name is "<<moviename;
actiongraph.findmoviesbyactor(moviename);
}
else if(c2 == 1){
cout<<"Enter movie name -> ";
string moviename;
clearInputBuffer();
getline(cin , moviename);
actiongraph.findmoviewithsameactors(moviename);
}
else{
cout<<"Wrong choice entered";
}
break;
case 4:
cout<<"\n1) Search movie of same director by movie name 2) Seach movie by director name\n-> ";
cin>>c2;
// clearInputBuffer();
if(c2 == 2){
cout<<"Enter director name -> ";
string moviename;
clearInputBuffer();
getline(cin , moviename);
actiongraph.findmoviesbydirector(moviename);
}
else if(c2 == 1){
cout<<"Enter movie name -> ";
string moviename;
clearInputBuffer();
getline(cin , moviename);
actiongraph.findmoviebysamedirector(moviename);
}
else{
cout<<"Wrong choice entered";
}
break;
case 5:
cout<<"\n1) Search movie of same year by movie name 2) Seach movie by year\n-> ";
cin>>c2;
//clearInputBuffer();
if(c2 == 2){
cout<<"Enter year -> ";
string moviename;
clearInputBuffer();
getline(cin , moviename);
actiongraph.findmoviesbyyear(moviename);
}
else if(c2 == 1){
cout<<"Enter movie name -> ";
string moviename;
clearInputBuffer();
getline(cin , moviename);
actiongraph.findmoviebysameyear(moviename);
}
else{
cout<<"Wrong choice entered";
}
break;
default:
cout<<"\nWrong choice entered ///";
break;
}
}
else if(choice ==2){
continue;
}
else{
cout<<RED<<"You entered wrong input exiting"<<RESET<<endl;
break;
}
}
return 0;
}
