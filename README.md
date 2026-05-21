# Tower of Hanoi — Graph Search Algorithms

Bu proje, **Tower of Hanoi** problemini çeşitli graf arama algoritmaları kullanarak çözen bir C uygulamasıdır. Kod, farklı arama problemlerine kolayca uyarlanabilecek şekilde modüler bir mimari üzerine inşa edilmiştir.

---

## Dosya Yapısı

```
.
├── GRAPH_SEARCH.c        # Ana program (main fonksiyonu)
├── GRAPH_SEARCH.h        # Tüm fonksiyon bildirimleri ve sabitler
├── data_types.h          # Veri yapıları (State, Node, Queue, Actions, Methods)
├── SpecificToProblem.c   # Probleme özgü fonksiyonlar (durum oluşturma, geçiş modeli, sezgisel fonksiyon)
├── Standart_Search.c     # Genel arama algoritmaları (BFS, DFS, A*, vb.)
├── HashTable.c           # Hash tablosu implementasyonu (keşfedilmiş düğümler için)
└── HashTable.h           # Hash tablosu bildirimleri ve sabitleri
```

---

## Veri Yapıları

### `State`
Hanoi probleminin anlık durumunu temsil eder.

| Alan | Tip | Açıklama |
|---|---|---|
| `num_disks` | `int` | Disk sayısı (3, 5 veya 9) |
| `disk[MAX_DISKS]` | `int[]` | Her diskin hangi kazıkta olduğu (0=A, 1=B, 2=C) |
| `h_n` | `float` | Sezgisel fonksiyon değeri h(n) |

### `Node`
Arama ağacındaki bir düğümü temsil eder.

| Alan | Tip | Açıklama |
|---|---|---|
| `state` | `State` | Düğümün durumu |
| `path_cost` | `float` | Kök düğümden bu düğüme toplam maliyet g(n) |
| `action` | `enum ACTIONS` | Ebeveyn düğümden bu düğüme ulaşan eylem |
| `parent` | `Node*` | Ebeveyn düğüm işaretçisi |
| `Number_of_Child` | `int` | Çocuk düğüm sayısı (DFS bellek yönetimi için) |

### `enum ACTIONS`
Hanoi'de mümkün olan 6 hareket:
`Move_A_B`, `Move_A_C`, `Move_B_A`, `Move_B_C`, `Move_C_A`, `Move_C_B`

### `enum METHODS`
Desteklenen arama algoritmaları:

| Değer | Algoritma |
|---|---|
| 1 | Breadth-First Search (BFS) |
| 2 | Uniform-Cost Search (UCS) |
| 3 | Depth-First Search (DFS) |
| 4 | Depth-Limited Search |
| 5 | Iterative Deepening Search |
| 6 | Greedy Search |
| 7 | A\* Search |
| 8 | Generalized A\* Search |

---

## Algoritma Açıklamaları

### Breadth-First Search (BFS)
FIFO kuyruğu kullanır. Tüm düğümleri seviye seviye genişletir. En az adımlı (sığ) çözümü garanti eder.

### Uniform-Cost Search (UCS)
Düğümleri `g(n)` (yol maliyeti) değerine göre sıralı öncelik kuyruğunda tutar. Aynı adım maliyetleri için BFS ile eşdeğerdir.

### Depth-First Search (DFS)
LIFO yığını kullanır. Bellek açısından verimlidir fakat optimum çözümü garanti etmez.

### Depth-Limited Search
DFS'in belirli bir derinlik limiti (`Max_Level`) ile kısıtlanmış halidir.

### Iterative Deepening Search (IDS)
Derinlik limitini 0'dan başlatarak her iterasyonda 1 artırarak DFS çalıştırır. BFS'in optimallik garantisini DFS'in düşük bellek kullanımıyla birleştirir.

### Greedy Search
Düğümleri yalnızca `h(n)` (sezgisel değer) ile sıralar. Hedefe en yakın görünen düğümü önce genişletir. Hızlıdır fakat optimal olmayabilir.

### A\* Search
Düğümleri `f(n) = g(n) + h(n)` değerine göre sıralar. Sezgisel fonksiyon kabul edilebilir (admissible) olduğu sürece optimal çözümü garanti eder.

### Generalized A\* Search
`f(n) = g(n) + α * h(n)` formülünü kullanır. `α` parametresiyle hız ve optimallik arasında denge kurulabilir:
- `α = 0` → Uniform-Cost Search
- `α = 1` → Standart A\*
- `α > 1` → Greedy'ye yaklaşır (daha hızlı ama optimal olmayabilir)

---

## Sezgisel Fonksiyon

`Compute_Heuristic_Function()` fonksiyonu, **ağırlıklı yanlış yerleştirilmiş disk** sezgisini kullanır:

```
h(n) = Σ 2^i   (yanlış kazıkta olan her disk i için)
```

Büyük diskler daha ağır penalize edilir. Bu sezgisel fonksiyon kabul edilebilirdir (admissible), dolayısıyla A\* algoritmasında optimal çözüm garantisini korur.

---

## Hash Tablosu

Keşfedilmiş düğümlerin hızlı aranması için açık adreslemeli (open addressing) bir hash tablosu kullanılmaktadır.

Temel özellikler:
- Tablo boyutu her zaman asal sayı olarak tutulur.
- Doluluk oranı `HASH_TABLE_INCREASING_RATE` (%70) aşıldığında tablo otomatik olarak 2 katına çıkarılır.
- Her durum için benzersiz anahtar `Generate_HashTable_Key()` ile oluşturulur (disk konumlarını karakter dizisine dönüştürür, örn. `"0002211"`).

İlgili sabitler (`HashTable.h`):

| Sabit | Varsayılan Değer | Açıklama |
|---|---|---|
| `HASH_TABLE_BASED_SIZE` | 10007 | Başlangıç tablo boyutu |
| `HASH_TABLE_INCREASING_RATE` | 70 | Yeniden boyutlandırma eşiği (%) |
| `MAX_KEY_SIZE` | 20 | Maksimum anahtar uzunluğu |

---

## Önemli Sabitler (`GRAPH_SEARCH.h`)

| Sabit | Değer | Açıklama |
|---|---|---|
| `PEG_NUMBER` | 3 | Kazık sayısı |
| `ACTION_COUNT` | 6 | Mümkün eylem sayısı |
| `PREDETERMINED_GOAL_STATE` | 1 | 1 ise hedef durum kullanıcı tarafından belirlenir |
| `MAX_SEARCHED_NODE` | 20000000 | Maksimum aranacak düğüm sayısı |
| `MAX_DISKS` | 9 | Maksimum disk sayısı |

---

## Derleme ve Çalıştırma

```bash
gcc GRAPH_SEARCH.c Standart_Search.c SpecificToProblem.c HashTable.c -o hanoi -lm
./hanoi
```

Örnek çalıştırma akışı:

```
1 --> Breast-First Search
...
7 --> A* Search
Select a method to solve the problem: 7

======== SELECTION OF INITIAL STATE ===============
Enter the number of disks (3, 5 or 9): 3

======== SELECTION OF GOAL STATE ===============
(Otomatik olarak tüm diskler C kazığında olacak şekilde ayarlanır)
```

---

## Farklı Bir Probleme Uyarlama

Bu framework başka arama problemleri için de kullanılabilir. Uyarlamak için yalnızca şu dosyalar güncellenir:

1. **`data_types.h`** — `State` yapısını ve `ACTIONS` enum'ını yeni probleme göre tanımla.
2. **`SpecificToProblem.c`** — Aşağıdaki 6 fonksiyonu yeni probleme göre uygula:
   - `Create_State()` — Başlangıç ve hedef durumu oluştur.
   - `Print_State()` — Durumu ekrana yazdır.
   - `Print_Action()` — Eylemi ekrana yazdır.
   - `Result()` — Geçiş modelini uygula.
   - `Compute_Heuristic_Function()` — h(n) hesapla.
   - `Goal_Test()` — Hedef testi yap.
3. **`HashTable.c`** — `Generate_HashTable_Key()` fonksiyonunu yeni `State` yapısına göre güncelle.
4. **`GRAPH_SEARCH.h`** — `ACTION_COUNT`, `PREDETERMINED_GOAL_STATE` gibi sabitleri güncelle.

`Standart_Search.c` ve `HashTable.c`'nin geri kalanına dokunmana gerek yok.

---

## Çıktı Bilgileri

Her arama tamamlandığında şu bilgiler gösterilir:

- Aranan düğüm sayısı (goal test geçen)
- Üretilen düğüm sayısı
- Bellekteki düğüm sayısı
- Toplam yol maliyeti
- Çözüm yolu (başlangıçtan hedefe her durum ve uygulanan eylem)
