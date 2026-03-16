import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import MiniBatchKMeans
from PIL import Image
import os

os.makedirs('../images', exist_ok=True)

print("=" * 70)
print("СЖАТИЕ ЦВЕТОВОЙ ПАЛИТРЫ ИЗОБРАЖЕНИЯ С ПОМОЩЬЮ K-MEANS")
print("=" * 70)

image_path = input("Введите путь к изображению (по умолчанию ../data/image.jpg): ") or '../data/image.jpg'
n_colors = int(input("Введите количество цветов для сжатия (по умолчанию 8): ") or 8)

try:
    img = Image.open(image_path)
    img_array = np.array(img)
    original_shape = img_array.shape
    
    print(f"\nЗагружено изображение: {image_path}")
    print(f"Размер изображения: {original_shape}")
    print(f"Количество цветов для сжатия: {n_colors}")
    
    if len(original_shape) == 3:
        h, w, c = original_shape
        pixels = img_array.reshape(-1, c)
    else:
        h, w = original_shape
        pixels = img_array.reshape(-1, 1)
        pixels = np.repeat(pixels, 3, axis=1)
    
    print(f"Всего пикселей: {len(pixels)}")
    print(f"Уникальных цветов до сжатия: {len(np.unique(pixels, axis=0))}")
    
    print("\n" + "=" * 70)
    print("ОБУЧЕНИЕ K-MEANS ДЛЯ СЖАТИЯ ПАЛИТРЫ")
    print("=" * 70)
    
    sample_size = min(100000, len(pixels))
    pixel_sample = pixels[np.random.choice(len(pixels), sample_size, replace=False)]
    
    kmeans = MiniBatchKMeans(n_clusters=n_colors, random_state=42, batch_size=10000, n_init=3)
    kmeans.fit(pixel_sample)
    centers = kmeans.cluster_centers_.astype(np.uint8)
    
    labels = kmeans.predict(pixels)
    
    print("Центры кластеров (новые цвета):")
    for i, center in enumerate(centers):
        print(f"  Кластер {i}: RGB{tuple(center)}")
    
    print(f"\nРаспределение пикселей по кластерам:")
    unique, counts = np.unique(labels, return_counts=True)
    for i, count in zip(unique, counts):
        print(f"  Кластер {i}: {count} пикселей ({count/len(pixels)*100:.2f}%)")
    
    compressed_pixels = centers[labels]
    compressed_array = compressed_pixels.reshape(original_shape)
    compressed_img = Image.fromarray(compressed_array.astype(np.uint8))
    
    print("\n" + "=" * 70)
    print("ВИЗУАЛИЗАЦИЯ РЕЗУЛЬТАТОВ")
    print("=" * 70)
    
    fig = plt.figure(figsize=(20, 10))
    
    ax1 = plt.subplot(2, 3, 1)
    ax1.imshow(img)
    ax1.set_title(f'Исходное изображение\n{original_shape[0]}x{original_shape[1]} пикселей', fontsize=12)
    ax1.axis('off')
    
    ax2 = plt.subplot(2, 3, 2)
    ax2.imshow(compressed_img)
    ax2.set_title(f'Сжатое изображение\n{n_colors} цветов', fontsize=12)
    ax2.axis('off')
    
    ax3 = plt.subplot(2, 3, 3)
    palette = np.zeros((50 * n_colors, 100, 3), dtype=np.uint8)
    for i in range(n_colors):
        palette[i*50:(i+1)*50, :] = centers[i]
    ax3.imshow(palette)
    ax3.set_title('Палитра цветов после сжатия', fontsize=12)
    ax3.set_yticks([i*50 + 25 for i in range(n_colors)])
    ax3.set_yticklabels([f'Цвет {i}' for i in range(n_colors)])
    ax3.set_xticks([])
    
    ax4 = plt.subplot(2, 3, 4)
    sample_size_vis = min(5000, len(pixels))
    random_sample = np.random.choice(len(pixels), sample_size_vis, replace=False)
    colors_original = pixels[random_sample] / 255.0
    ax4.scatter(colors_original[:, 0], colors_original[:, 1], 
               c=colors_original, alpha=0.6, s=5)
    ax4.set_xlabel('Red')
    ax4.set_ylabel('Green')
    ax4.set_title('Цветовое пространство (R-G) до сжатия', fontsize=12)
    ax4.grid(True, alpha=0.3)
    
    ax5 = plt.subplot(2, 3, 5)
    colors_compressed = compressed_pixels[random_sample] / 255.0
    ax5.scatter(colors_compressed[:, 0], colors_compressed[:, 1], 
               c=colors_compressed, alpha=0.6, s=5)
    ax5.set_xlabel('Red')
    ax5.set_ylabel('Green')
    ax5.set_title('Цветовое пространство (R-G) после сжатия', fontsize=12)
    ax5.grid(True, alpha=0.3)
    
    ax6 = plt.subplot(2, 3, 6)
    sizes = [np.sum(labels == i) for i in range(n_colors)]
    colors_hex = ['#%02x%02x%02x' % tuple(center) for center in centers]
    wedges, texts, autotexts = ax6.pie(sizes, labels=[f'{i}' for i in range(n_colors)], 
                                        colors=colors_hex, autopct='%1.1f%%', startangle=90)
    ax6.set_title('Распределение пикселей по цветам', fontsize=12)
    
    plt.suptitle(f'Сжатие цветовой палитры с {len(np.unique(pixels, axis=0))} до {n_colors} цветов', 
                fontsize=16, y=1.02)
    plt.tight_layout()
    plt.savefig('../images/color_compression.png', dpi=150, bbox_inches='tight')
    plt.show()
    
    print("\n" + "=" * 70)
    print("ДОПОЛНИТЕЛЬНАЯ ВИЗУАЛИЗАЦИЯ: СРАВНЕНИЕ ПРИ РАЗНОМ КОЛИЧЕСТВЕ ЦВЕТОВ")
    print("=" * 70)
    
    fig, axes = plt.subplots(2, 3, figsize=(18, 12))
    axes = axes.flatten()
    
    test_colors = [2, 4, 8, 16, 32, 64]
    
    crop_size = 300
    center_y, center_x = h // 2, w // 2
    crop_img = img_array[center_y-crop_size//2:center_y+crop_size//2, 
                         center_x-crop_size//2:center_x+crop_size//2]
    crop_pixels = crop_img.reshape(-1, 3)
    
    for idx, n in enumerate(test_colors):
        print(f"  Обработка {n} цветов...")
        
        sample_for_training = pixels[np.random.choice(len(pixels), min(50000, len(pixels)), replace=False)]
        
        kmeans_test = MiniBatchKMeans(n_clusters=n, random_state=42, batch_size=10000, n_init=3)
        kmeans_test.fit(sample_for_training)
        centers_test = kmeans_test.cluster_centers_.astype(np.uint8)
        
        crop_labels = kmeans_test.predict(crop_pixels)
        crop_compressed = centers_test[crop_labels].reshape(crop_img.shape)
        
        axes[idx].imshow(crop_compressed)
        axes[idx].set_title(f'{n} цветов', fontsize=12, fontweight='bold')
        axes[idx].axis('off')
        
        axes[idx].text(0.5, -0.05, f'Уникальных цветов: {len(np.unique(centers_test, axis=0))}', 
                      transform=axes[idx].transAxes, ha='center', fontsize=10)
    
    plt.suptitle('Сравнение сжатия с разным количеством цветов (центральная область)', 
                fontsize=16, y=0.98)
    plt.tight_layout()
    plt.savefig('../images/color_compression_comparison.png', dpi=150, bbox_inches='tight')
    plt.show()
    
    print("\n" + "=" * 70)
    print("СОХРАНЕНИЕ РЕЗУЛЬТАТОВ")
    print("=" * 70)
    
    original_filename = os.path.basename(image_path)
    name_without_ext = os.path.splitext(original_filename)[0]
    
    compressed_img.save(f'../images/{name_without_ext}_compressed_{n_colors}colors.jpg', quality=95)
    print(f"Сжатое изображение сохранено: ../images/{name_without_ext}_compressed_{n_colors}colors.jpg")
    
    palette_img = Image.fromarray(palette)
    palette_img.save(f'../images/{name_without_ext}_palette_{n_colors}colors.jpg')
    print(f"Палитра цветов сохранена: ../images/{name_without_ext}_palette_{n_colors}colors.jpg")
    
    print("\n" + "=" * 70)
    print("АНАЛИЗ ЭФФЕКТИВНОСТИ СЖАТИЯ")
    print("=" * 70)
    
    original_size = os.path.getsize(image_path) / 1024
    compressed_path = f'../images/{name_without_ext}_compressed_{n_colors}colors.jpg'
    compressed_size = os.path.getsize(compressed_path) / 1024 if os.path.exists(compressed_path) else 0
    
    print(f"Размер исходного файла: {original_size:.2f} КБ")
    print(f"Размер сжатого файла: {compressed_size:.2f} КБ")
    if original_size > 0:
        print(f"Сжатие: {compressed_size/original_size*100:.1f}% от оригинала")
        print(f"Экономия места: {100 - compressed_size/original_size*100:.1f}%")
    
    print("\n" + "=" * 70)
    print("ВЫВОДЫ")
    print("=" * 70)
    
    print(f"""
СЖАТИЕ ЦВЕТОВОЙ ПАЛИТРЫ С ПОМОЩЬЮ K-MEANS:

1. Исходное изображение содержало {len(np.unique(pixels, axis=0))} уникальных цветов
2. После сжатия осталось {n_colors} цветов (центры кластеров)
3. Каждый пиксель заменен на цвет ближайшего центра кластера

ВИЗУАЛЬНОЕ КАЧЕСТВО:
- При {n_colors} цветах изображение {'сохраняет основные черты' if n_colors >= 8 else 'сильно упрощено'}
- Меньшее количество цветов создает плакатный эффект
- Большее количество цветов лучше сохраняет градиенты

ЭФФЕКТИВНОСТЬ:
- Сжатие цветовой палитры позволяет значительно уменьшить размер файла
- При сохранении в JPEG дополнительное сжатие может быть применено
- Оптимальное количество цветов зависит от конкретного изображения

ПРИМЕНЕНИЕ:
- Создание художественных эффектов (постеризация)
- Уменьшение размера изображений для веба
- Предобработка для компьютерного зрения
- Стилизация изображений
""")
    
except FileNotFoundError:
    print(f"Ошибка: Файл {image_path} не найден!")
    print("Создаю тестовое изображение...")
    
    x = np.linspace(0, 1, 300)
    y = np.linspace(0, 1, 300)
    X, Y = np.meshgrid(x, y)
    
    R = np.sin(X * 10) * 255
    G = np.cos(Y * 10) * 255
    B = (X + Y) * 255 / 2
    
    test_img_array = np.stack([R, G, B], axis=2).astype(np.uint8)
    test_img = Image.fromarray(test_img_array)
    test_img.save('../data/test_image.jpg')
    
    print("Тестовое изображение создано: ../data/test_image.jpg")
    print("Запустите программу снова, указав этот путь.")
