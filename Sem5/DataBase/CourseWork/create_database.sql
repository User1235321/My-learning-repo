-- Создание таблицы товаров
CREATE TABLE goods (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    priority INTEGER NOT NULL DEFAULT 1
);

-- Создание таблицы заявок
CREATE TABLE sales (
    id SERIAL PRIMARY KEY,
    good_id INTEGER NOT NULL,
    good_count INTEGER NOT NULL,
    create_date DATE NOT NULL DEFAULT CURRENT_DATE,
    FOREIGN KEY (good_id) REFERENCES goods(id) ON DELETE CASCADE
);

-- Создание таблицы для склада 1
CREATE TABLE warehouse1 (
    id SERIAL PRIMARY KEY,
    good_id INTEGER NOT NULL UNIQUE,
    good_count INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY (good_id) REFERENCES goods(id) ON DELETE CASCADE
);

-- Создание таблицы для склада 2
CREATE TABLE warehouse2 (
    id SERIAL PRIMARY KEY,
    good_id INTEGER NOT NULL UNIQUE,
    good_count INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY (good_id) REFERENCES goods(id) ON DELETE CASCADE
);

-- Триггерная функция для проверки наличия товара перед списанием
CREATE OR REPLACE FUNCTION check_warehouse_stock()
RETURNS TRIGGER AS $$
DECLARE
    total_stock INTEGER;
BEGIN
    -- Проверяем общее количество товара на обоих складах
    SELECT COALESCE(w1.good_count, 0) + COALESCE(w2.good_count, 0) INTO total_stock
    FROM goods g
    LEFT JOIN warehouse1 w1 ON g.id = w1.good_id
    LEFT JOIN warehouse2 w2 ON g.id = w2.good_id
    WHERE g.id = NEW.good_id;
    
    IF total_stock < NEW.good_count THEN
        RAISE EXCEPTION 'Недостаточно товара на складах. Доступно: %, запрошено: %', total_stock, NEW.good_count;
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Триггер для проверки перед вставкой заявки
CREATE TRIGGER tr_check_stock_before_sale
    BEFORE INSERT ON sales
    FOR EACH ROW
    EXECUTE FUNCTION check_warehouse_stock();

-- Триггерная функция для автоматического списания товара
CREATE OR REPLACE FUNCTION process_sale()
RETURNS TRIGGER AS $$
DECLARE
    warehouse1_stock INTEGER;
    warehouse2_stock INTEGER;
    remaining INTEGER;
BEGIN
    -- Получаем количество товара на складе 1
    SELECT COALESCE(good_count, 0) INTO warehouse1_stock
    FROM warehouse1 WHERE good_id = NEW.good_id;
    
    remaining := NEW.good_count;
    
    -- Сначала списываем со склада 1
    IF warehouse1_stock > 0 THEN
        IF warehouse1_stock >= remaining THEN
            UPDATE warehouse1 SET good_count = good_count - remaining WHERE good_id = NEW.good_id;
            remaining := 0;
        ELSE
            UPDATE warehouse1 SET good_count = 0 WHERE good_id = NEW.good_id;
            remaining := remaining - warehouse1_stock;
        END IF;
    END IF;
    
    -- Если осталось, списываем со склада 2
    IF remaining > 0 THEN
        SELECT COALESCE(good_count, 0) INTO warehouse2_stock
        FROM warehouse2 WHERE good_id = NEW.good_id;
        
        IF warehouse2_stock < remaining THEN
            RAISE EXCEPTION 'Недостаточно товара на складах после списания';
        END IF;
        
        UPDATE warehouse2 SET good_count = good_count - remaining WHERE good_id = NEW.good_id;
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Триггер для автоматического списания товара после вставки заявки
CREATE TRIGGER tr_process_sale
    AFTER INSERT ON sales
    FOR EACH ROW
    EXECUTE FUNCTION process_sale();

-- Вставка тестовых данных
INSERT INTO goods (name, priority) VALUES 
    ('Молоко', 1),
    ('Хлеб', 2),
    ('Яблоки', 3),
    ('Сахар', 4),
    ('Соль', 5);

INSERT INTO warehouse1 (good_id, good_count) VALUES 
    (1, 100),
    (2, 150),
    (3, 200);

INSERT INTO warehouse2 (good_id, good_count) VALUES 
    (1, 500),
    (2, 600),
    (3, 700),
    (4, 800),
    (5, 900);
