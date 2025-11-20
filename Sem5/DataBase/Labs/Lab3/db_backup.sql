--
-- PostgreSQL database dump
--

\restrict wP5bYjZvGbaPmK4Z9LCvXF1MalOrjyW6m4WitkrmF9xAXfz8a7cQ9S430pTBYMl

-- Dumped from database version 18.1 (Debian 18.1-1.pgdg12+2)
-- Dumped by pg_dump version 18.1 (Debian 18.1-1.pgdg12+2)

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

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: goods; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.goods (
    id integer NOT NULL,
    name text,
    priority integer
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
-- Name: warehouse2; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.warehouse2 (
    id integer NOT NULL,
    good_id integer,
    good_count integer
);


ALTER TABLE public.warehouse2 OWNER TO postgres;

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

COPY public.goods (id, name, priority) FROM stdin;
1	Процессор Bintel	1
2	Процессор AMV	1
3	Видеокарта NVIDIO	1
4	Видеокарта RAZEN	1
5	Материнская плата ASPAPER	2
6	Материнская плата TERABYTE	2
7	Оперативная память ARDOR WORKING	2
8	Оперативная память QUEENSTON	2
9	Мышь ARDOR WORKING	3
10	Мышь GOGITECH	3
11	Клавиатура ARDOR WORKING	3
12	Клавиатура BEXP	3
13	Наушники ARDOR WORKING	4
14	Наушники EARTHPODS	4
15	Монитор ARDOR WORKING	3
16	Монитор MSA	3
17	Диски с операционной системой Шиндовс	5
20	Диски с операционной системой TempleOS	5
27	Коврик для мыши ARDOR WORKING	6
28	TEST_DATA	100
29	TEST_DATA_2	100
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

SELECT pg_catalog.setval('public.sales_id_seq', 22, true);


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
-- PostgreSQL database dump complete
--

\unrestrict wP5bYjZvGbaPmK4Z9LCvXF1MalOrjyW6m4WitkrmF9xAXfz8a7cQ9S430pTBYMl

