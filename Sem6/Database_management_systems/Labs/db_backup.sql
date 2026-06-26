--
-- PostgreSQL database dump
--

\restrict Q82jIyHfZogjlqv52TiEZZawX1q9YgCDG7V903NOfWjFbXWQaYgSJONaoqdrVND

-- Dumped from database version 17.9 (Debian 17.9-0+deb13u1)
-- Dumped by pg_dump version 17.9 (Debian 17.9-0+deb13u1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: check_goods_integrity(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.check_goods_integrity() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    -- Проверка приоритета
    IF NEW.priority < 1 OR NEW.priority > 10 THEN
        RAISE EXCEPTION 'Приоритет товара должен быть от 1 до 10 (указан: %)', NEW.priority;
    END IF;
    
    -- Проверка количества
    IF NEW.quantity IS NOT NULL AND NEW.quantity < 0 THEN
        RAISE EXCEPTION 'Количество товара не может быть отрицательным (указано: %)', NEW.quantity;
    END IF;
    
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.check_goods_integrity() OWNER TO postgres;

--
-- Name: check_unique_goods_name(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.check_unique_goods_name() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    IF EXISTS (
        SELECT 1 FROM goods 
        WHERE name = NEW.name AND id != NEW.id
    ) THEN
        RAISE EXCEPTION 'Товар с названием "%" уже существует.', NEW.name;
    END IF;
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.check_unique_goods_name() OWNER TO postgres;

--
-- Name: get_goods_for_transport(); Type: PROCEDURE; Schema: public; Owner: postgres
--

CREATE PROCEDURE public.get_goods_for_transport()
    LANGUAGE sql
    AS $$
SELECT g.id, g.name, g.priority, 
       (100 - COALESCE(w1.good_count, 0)) as need_to_transport
FROM goods g
LEFT JOIN warehouse1 w1 ON g.id = w1.good_id
WHERE g.priority > 7
ORDER BY g.priority DESC;
$$;


ALTER PROCEDURE public.get_goods_for_transport() OWNER TO postgres;

--
-- Name: insert_into_sales_info(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.insert_into_sales_info() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
    v_good_id INTEGER;
BEGIN
    -- Проверка обязательных полей
    IF NEW.good_name IS NULL THEN
        RAISE EXCEPTION 'Поле good_name не может быть пустым';
    END IF;
    
    IF NEW.sold_count IS NULL THEN
        RAISE EXCEPTION 'Поле sold_count не может быть пустым';
    END IF;
    
    -- Поиск существующего товара по имени
    SELECT id INTO v_good_id FROM goods WHERE name = NEW.good_name;
    
    -- Если товар не найден, создаем новый
    IF v_good_id IS NULL THEN
        INSERT INTO goods (name, priority, quantity) 
        VALUES (NEW.good_name, COALESCE(NEW.priority, 5), 0) 
        RETURNING id INTO v_good_id;
        
        RAISE NOTICE 'Создан новый товар: % с id = %', NEW.good_name, v_good_id;
    END IF;
    
    -- Вставка записи о продаже
    INSERT INTO sales (good_id, good_count, create_date)
    VALUES (
        v_good_id, 
        NEW.sold_count, 
        COALESCE(NEW.create_date, CURRENT_DATE)
    );
    
    -- Установка значений для возвращаемой строки
    NEW.sale_id := currval('sales_id_seq');
    NEW.good_id := v_good_id;
    NEW.total_stock := 0; -- Будет пересчитано при следующем SELECT
    
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.insert_into_sales_info() OWNER TO postgres;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: goods; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.goods (
    id integer NOT NULL,
    name text,
    priority integer,
    quantity integer DEFAULT 100
);


ALTER TABLE public.goods OWNER TO postgres;

--
-- Name: warehouse1; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.warehouse1 (
    id integer NOT NULL,
    good_id integer,
    good_count integer
);


ALTER TABLE public.warehouse1 OWNER TO postgres;

--
-- Name: firstview; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW public.firstview AS
 SELECT goods.id,
    goods.name,
    goods.priority,
    warehouse1.good_count
   FROM (public.goods
     LEFT JOIN public.warehouse1 ON ((warehouse1.good_id = goods.id)))
  WHERE (warehouse1.good_count > 10);


ALTER VIEW public.firstview OWNER TO postgres;

--
-- Name: goods_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.goods_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.goods_id_seq OWNER TO postgres;

--
-- Name: goods_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.goods_id_seq OWNED BY public.goods.id;


--
-- Name: high_priority_goods; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW public.high_priority_goods AS
 SELECT id,
    name,
    priority
   FROM public.goods
  WHERE (priority > 5)
  ORDER BY priority DESC;


ALTER VIEW public.high_priority_goods OWNER TO postgres;

--
-- Name: VIEW high_priority_goods; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON VIEW public.high_priority_goods IS 'Товары с высоким приоритетом (более 5)';


--
-- Name: premium_goods; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW public.premium_goods AS
 SELECT id,
    name,
    priority,
    quantity
   FROM public.goods
  WHERE (priority >= 8)
  WITH CASCADED CHECK OPTION;


ALTER VIEW public.premium_goods OWNER TO postgres;

--
-- Name: VIEW premium_goods; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON VIEW public.premium_goods IS 'Премиум товары (приоритет 8 и выше) с CHECK OPTION';


--
-- Name: sales; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.sales (
    id integer NOT NULL,
    good_id integer,
    good_count integer,
    create_date date
);


ALTER TABLE public.sales OWNER TO postgres;

--
-- Name: sales_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.sales_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.sales_id_seq OWNER TO postgres;

--
-- Name: sales_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.sales_id_seq OWNED BY public.sales.id;


--
-- Name: warehouse2; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.warehouse2 (
    id integer NOT NULL,
    good_id integer,
    good_count integer
);


ALTER TABLE public.warehouse2 OWNER TO postgres;

--
-- Name: sales_info; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW public.sales_info AS
 SELECT s.id AS sale_id,
    g.id AS good_id,
    g.name AS good_name,
    g.priority,
    s.good_count AS sold_count,
    s.create_date,
    COALESCE(w1.good_count, 0) AS warehouse1_stock,
    COALESCE(w2.good_count, 0) AS warehouse2_stock,
    (COALESCE(w1.good_count, 0) + COALESCE(w2.good_count, 0)) AS total_stock
   FROM (((public.sales s
     JOIN public.goods g ON ((s.good_id = g.id)))
     LEFT JOIN public.warehouse1 w1 ON ((g.id = w1.good_id)))
     LEFT JOIN public.warehouse2 w2 ON ((g.id = w2.good_id)))
  ORDER BY s.create_date DESC;


ALTER VIEW public.sales_info OWNER TO postgres;

--
-- Name: VIEW sales_info; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON VIEW public.sales_info IS 'Информация о продажах с остатками на складах';


--
-- Name: secondview; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW public.secondview AS
 SELECT goods.id,
    goods.name,
    sum(sales.good_count) AS total_sold
   FROM (public.goods
     JOIN public.sales ON ((goods.id = sales.good_id)))
  WHERE (EXTRACT(month FROM sales.create_date) = '9'::numeric)
  GROUP BY goods.id, goods.name
 LIMIT 5;


ALTER VIEW public.secondview OWNER TO postgres;

--
-- Name: test_anomalies; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.test_anomalies (
    id integer NOT NULL,
    value integer,
    description text
);


ALTER TABLE public.test_anomalies OWNER TO postgres;

--
-- Name: warehouse1_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.warehouse1_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.warehouse1_id_seq OWNER TO postgres;

--
-- Name: warehouse1_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.warehouse1_id_seq OWNED BY public.warehouse1.id;


--
-- Name: warehouse2_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.warehouse2_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.warehouse2_id_seq OWNER TO postgres;

--
-- Name: warehouse2_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.warehouse2_id_seq OWNED BY public.warehouse2.id;


--
-- Name: goods id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.goods ALTER COLUMN id SET DEFAULT nextval('public.goods_id_seq'::regclass);


--
-- Name: sales id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sales ALTER COLUMN id SET DEFAULT nextval('public.sales_id_seq'::regclass);


--
-- Name: warehouse1 id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.warehouse1 ALTER COLUMN id SET DEFAULT nextval('public.warehouse1_id_seq'::regclass);


--
-- Name: warehouse2 id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.warehouse2 ALTER COLUMN id SET DEFAULT nextval('public.warehouse2_id_seq'::regclass);


--
-- Data for Name: goods; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.goods (id, name, priority, quantity) FROM stdin;
3	Видеокарта NVIDIO	1	100
4	Видеокарта RAZEN	1	100
5	Материнская плата ASPAPER	2	100
6	Материнская плата TERABYTE	2	100
7	Оперативная память ARDOR WORKING	2	100
8	Оперативная память QUEENSTON	2	100
9	Мышь ARDOR WORKING	3	100
10	Мышь GOGITECH	3	100
11	Клавиатура ARDOR WORKING	3	100
12	Клавиатура BEXP	3	100
13	Наушники ARDOR WORKING	4	100
14	Наушники EARTHPODS	4	100
15	Монитор ARDOR WORKING	3	100
16	Монитор MSA	3	100
17	Диски с операционной системой Шиндовс	5	100
20	Диски с операционной системой TempleOS	5	100
27	Коврик для мыши ARDOR WORKING	6	50
28	TEST_DATA	100	50
29	TEST_DATA_2	100	50
30	Фантомный товар	6	50
2	Процессор AMV	1	100
31	Новый товар	5	50
1	Процессор Bintel	6	100
32	Премиум товар 1	10	100
\.


--
-- Data for Name: sales; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.sales (id, good_id, good_count, create_date) FROM stdin;
1	15	3	2025-09-01
2	6	10	2025-09-02
3	5	3	2025-09-03
4	12	12	2025-09-04
5	16	17	2025-09-04
6	2	14	2025-09-05
7	6	17	2025-09-06
8	12	14	2025-09-07
9	7	6	2025-09-10
10	9	15	2025-09-11
11	1	15	2025-09-12
12	10	12	2025-09-13
13	9	1	2025-09-14
14	10	7	2025-09-15
15	2	19	2025-09-16
16	3	6	2025-09-17
17	12	6	2025-09-18
18	4	16	2025-09-19
19	11	8	2025-09-20
20	7	9	2025-09-21
21	6	10	2025-09-22
22	17	200	2025-09-25
23	1	5	2026-02-27
\.


--
-- Data for Name: test_anomalies; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.test_anomalies (id, value, description) FROM stdin;
1	100	Тестовая запись 1
2	200	Тестовая запись 2
\.


--
-- Data for Name: warehouse1; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.warehouse1 (id, good_id, good_count) FROM stdin;
1	1	100
2	2	100
3	3	50
4	4	40
5	5	30
6	6	30
7	7	60
8	8	67
11	20	400
18	27	10
19	28	100
20	29	100
\.


--
-- Data for Name: warehouse2; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.warehouse2 (id, good_id, good_count) FROM stdin;
1	1	46
2	4	66
3	5	68
4	7	73
5	8	105
6	9	953
7	10	560
8	11	586
9	12	719
10	13	819
11	14	223
12	15	481
13	16	321
14	17	8000
15	20	8000
22	27	200
23	28	100
24	29	100
\.


--
-- Name: goods_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.goods_id_seq', 29, true);


--
-- Name: sales_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.sales_id_seq', 23, true);


--
-- Name: warehouse1_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.warehouse1_id_seq', 20, true);


--
-- Name: warehouse2_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.warehouse2_id_seq', 24, true);


--
-- Name: goods goods_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.goods
    ADD CONSTRAINT goods_pkey PRIMARY KEY (id);


--
-- Name: sales sales_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sales
    ADD CONSTRAINT sales_pkey PRIMARY KEY (id);


--
-- Name: test_anomalies test_anomalies_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.test_anomalies
    ADD CONSTRAINT test_anomalies_pkey PRIMARY KEY (id);


--
-- Name: warehouse1 warehouse1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.warehouse1
    ADD CONSTRAINT warehouse1_pkey PRIMARY KEY (id);


--
-- Name: warehouse2 warehouse2_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.warehouse2
    ADD CONSTRAINT warehouse2_pkey PRIMARY KEY (id);


--
-- Name: goods trg_check_goods_integrity; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER trg_check_goods_integrity BEFORE INSERT OR UPDATE ON public.goods FOR EACH ROW EXECUTE FUNCTION public.check_goods_integrity();


--
-- Name: goods trg_check_unique_goods_name; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER trg_check_unique_goods_name BEFORE INSERT OR UPDATE ON public.goods FOR EACH ROW EXECUTE FUNCTION public.check_unique_goods_name();


--
-- Name: sales_info trg_insert_sales_info; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER trg_insert_sales_info INSTEAD OF INSERT ON public.sales_info FOR EACH ROW EXECUTE FUNCTION public.insert_into_sales_info();


--
-- Name: sales sales_good_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sales
    ADD CONSTRAINT sales_good_id_fkey FOREIGN KEY (good_id) REFERENCES public.goods(id) ON DELETE CASCADE;


--
-- Name: warehouse1 warehouse1_good_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.warehouse1
    ADD CONSTRAINT warehouse1_good_id_fkey FOREIGN KEY (good_id) REFERENCES public.goods(id) ON DELETE CASCADE;


--
-- Name: warehouse2 warehouse2_good_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.warehouse2
    ADD CONSTRAINT warehouse2_good_id_fkey FOREIGN KEY (good_id) REFERENCES public.goods(id) ON DELETE CASCADE;


--
-- Name: SCHEMA public; Type: ACL; Schema: -; Owner: pg_database_owner
--

GRANT USAGE ON SCHEMA public TO test;


--
-- Name: COLUMN goods.id; Type: ACL; Schema: public; Owner: postgres
--

GRANT SELECT(id) ON TABLE public.goods TO test;


--
-- Name: COLUMN goods.name; Type: ACL; Schema: public; Owner: postgres
--

GRANT SELECT(name) ON TABLE public.goods TO test;


--
-- Name: COLUMN goods.priority; Type: ACL; Schema: public; Owner: postgres
--

GRANT SELECT(priority),UPDATE(priority) ON TABLE public.goods TO test;


--
-- Name: TABLE warehouse1; Type: ACL; Schema: public; Owner: postgres
--

GRANT SELECT ON TABLE public.warehouse1 TO test;


--
-- Name: SEQUENCE goods_id_seq; Type: ACL; Schema: public; Owner: postgres
--

GRANT USAGE ON SEQUENCE public.goods_id_seq TO test;


--
-- Name: TABLE high_priority_goods; Type: ACL; Schema: public; Owner: postgres
--

GRANT SELECT ON TABLE public.high_priority_goods TO test;


--
-- Name: TABLE premium_goods; Type: ACL; Schema: public; Owner: postgres
--

GRANT SELECT,INSERT,UPDATE ON TABLE public.premium_goods TO test;


--
-- Name: COLUMN premium_goods.priority; Type: ACL; Schema: public; Owner: postgres
--

GRANT UPDATE(priority) ON TABLE public.premium_goods TO goods_editor;


--
-- Name: TABLE sales; Type: ACL; Schema: public; Owner: postgres
--

GRANT SELECT,INSERT,UPDATE ON TABLE public.sales TO test;


--
-- Name: SEQUENCE sales_id_seq; Type: ACL; Schema: public; Owner: postgres
--

GRANT USAGE ON SEQUENCE public.sales_id_seq TO test;


--
-- Name: TABLE sales_info; Type: ACL; Schema: public; Owner: postgres
--

GRANT SELECT,INSERT ON TABLE public.sales_info TO test;


--
-- PostgreSQL database dump complete
--

\unrestrict Q82jIyHfZogjlqv52TiEZZawX1q9YgCDG7V903NOfWjFbXWQaYgSJONaoqdrVND
