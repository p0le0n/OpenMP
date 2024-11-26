import random

def generate_vectors(N, M, filename):
    with open(filename, 'w') as f:
        f.write(str(N) + " " + str(M) + "\n")
        for _ in range(N):
            vector = [str(random.randint(0, 100)) for _ in range(M)]
            f.write(" ".join(vector) + "\n")

if __name__ == "__main__":
    N = int(input("Введите количество векторов (N): "))
    M = int(input("Введите размерность векторов (M): "))
    filename = 'vectors.txt'
    generate_vectors(N, M, filename)
    print(f"Файл {filename} с {N} векторами размерности {M} создан.")
