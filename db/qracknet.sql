--
-- PostgreSQL database dump
--

-- Dumped from database version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)
-- Dumped by pg_dump version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)

-- Started on 2024-01-20 15:23:23 EST

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 202 (class 1259 OID 18946)
-- Name: jobStatusTypes; Type: TABLE; Schema: public; Owner: qracknet
--

CREATE TABLE public."jobStatusTypes" (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    name text NOT NULL
);


ALTER TABLE public."jobStatusTypes" OWNER TO qracknet;

--
-- TOC entry 211 (class 1259 OID 18988)
-- Name: jobStatusTypes_id_seq; Type: SEQUENCE; Schema: public; Owner: qracknet
--

CREATE SEQUENCE public."jobStatusTypes_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public."jobStatusTypes_id_seq" OWNER TO qracknet;

--
-- TOC entry 3019 (class 0 OID 0)
-- Dependencies: 211
-- Name: jobStatusTypes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public."jobStatusTypes_id_seq" OWNED BY public."jobStatusTypes".id;


--
-- TOC entry 203 (class 1259 OID 18952)
-- Name: jobs; Type: TABLE; Schema: public; Owner: qracknet
--

CREATE TABLE public.jobs (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    "jobStatusTypeId" integer NOT NULL,
    "statusMessage" text,
    "userId" integer NOT NULL
);


ALTER TABLE public.jobs OWNER TO qracknet;

--
-- TOC entry 210 (class 1259 OID 18985)
-- Name: jobs_id_seq; Type: SEQUENCE; Schema: public; Owner: qracknet
--

CREATE SEQUENCE public.jobs_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.jobs_id_seq OWNER TO qracknet;

--
-- TOC entry 3020 (class 0 OID 0)
-- Dependencies: 210
-- Name: jobs_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public.jobs_id_seq OWNED BY public.jobs.id;


--
-- TOC entry 204 (class 1259 OID 18958)
-- Name: outputTypes; Type: TABLE; Schema: public; Owner: qracknet
--

CREATE TABLE public."outputTypes" (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    name text NOT NULL
);


ALTER TABLE public."outputTypes" OWNER TO qracknet;

--
-- TOC entry 209 (class 1259 OID 18982)
-- Name: outputTypes_id_seq; Type: SEQUENCE; Schema: public; Owner: qracknet
--

CREATE SEQUENCE public."outputTypes_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public."outputTypes_id_seq" OWNER TO qracknet;

--
-- TOC entry 3021 (class 0 OID 0)
-- Dependencies: 209
-- Name: outputTypes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public."outputTypes_id_seq" OWNED BY public."outputTypes".id;


--
-- TOC entry 205 (class 1259 OID 18964)
-- Name: outputs; Type: TABLE; Schema: public; Owner: qracknet
--

CREATE TABLE public.outputs (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    "jobId" integer NOT NULL,
    "outputTypeId" integer NOT NULL,
    name text NOT NULL,
    value text NOT NULL
);


ALTER TABLE public.outputs OWNER TO qracknet;

--
-- TOC entry 208 (class 1259 OID 18979)
-- Name: outputs_id_seq; Type: SEQUENCE; Schema: public; Owner: qracknet
--

CREATE SEQUENCE public.outputs_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.outputs_id_seq OWNER TO qracknet;

--
-- TOC entry 3022 (class 0 OID 0)
-- Dependencies: 208
-- Name: outputs_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public.outputs_id_seq OWNED BY public.outputs.id;


--
-- TOC entry 206 (class 1259 OID 18970)
-- Name: users; Type: TABLE; Schema: public; Owner: qracknet
--

CREATE TABLE public.users (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    username text NOT NULL,
    "usernameNormal" text NOT NULL,
    "passwordHash" text NOT NULL,
    affiliation text NOT NULL,
    name text NOT NULL,
    email text NOT NULL,
    "recoveryToken" text,
    "recoveryTokenExpiration" date
);


ALTER TABLE public.users OWNER TO qracknet;

--
-- TOC entry 207 (class 1259 OID 18976)
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: qracknet
--

CREATE SEQUENCE public.users_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.users_id_seq OWNER TO qracknet;

--
-- TOC entry 3023 (class 0 OID 0)
-- Dependencies: 207
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- TOC entry 2859 (class 2604 OID 18990)
-- Name: jobStatusTypes id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public."jobStatusTypes" ALTER COLUMN id SET DEFAULT nextval('public."jobStatusTypes_id_seq"'::regclass);


--
-- TOC entry 2860 (class 2604 OID 18987)
-- Name: jobs id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.jobs ALTER COLUMN id SET DEFAULT nextval('public.jobs_id_seq'::regclass);


--
-- TOC entry 2861 (class 2604 OID 18984)
-- Name: outputTypes id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public."outputTypes" ALTER COLUMN id SET DEFAULT nextval('public."outputTypes_id_seq"'::regclass);


--
-- TOC entry 2862 (class 2604 OID 18981)
-- Name: outputs id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.outputs ALTER COLUMN id SET DEFAULT nextval('public.outputs_id_seq'::regclass);


--
-- TOC entry 2863 (class 2604 OID 18978)
-- Name: users id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- TOC entry 3004 (class 0 OID 18946)
-- Dependencies: 202
-- Data for Name: jobStatusTypes; Type: TABLE DATA; Schema: public; Owner: qracknet
--

COPY public."jobStatusTypes" (id, "createdAt", "updatedAt", name) FROM stdin;
1	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	SUCCESS
2	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	FAILED
3	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	RUNNING
4	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	IN QUEUE
\.


--
-- TOC entry 3005 (class 0 OID 18952)
-- Dependencies: 203
-- Data for Name: jobs; Type: TABLE DATA; Schema: public; Owner: qracknet
--

COPY public.jobs (id, "createdAt", "updatedAt", "jobStatusTypeId", "statusMessage", "userId") FROM stdin;
\.


--
-- TOC entry 3006 (class 0 OID 18958)
-- Dependencies: 204
-- Data for Name: outputTypes; Type: TABLE DATA; Schema: public; Owner: qracknet
--

COPY public."outputTypes" (id, "createdAt", "updatedAt", name) FROM stdin;
1	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	quid
2	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	bool
3	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	double
\.


--
-- TOC entry 3007 (class 0 OID 18964)
-- Dependencies: 205
-- Data for Name: outputs; Type: TABLE DATA; Schema: public; Owner: qracknet
--

COPY public.outputs (id, "createdAt", "updatedAt", "jobId", "outputTypeId", name, value) FROM stdin;
\.


--
-- TOC entry 3008 (class 0 OID 18970)
-- Dependencies: 206
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: qracknet
--

COPY public.users (id, "createdAt", "updatedAt", username, "usernameNormal", "passwordHash", affiliation, name, email, "recoveryToken", "recoveryTokenExpiration") FROM stdin;
\.


--
-- TOC entry 3024 (class 0 OID 0)
-- Dependencies: 211
-- Name: jobStatusTypes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public."jobStatusTypes_id_seq"', 1, false);


--
-- TOC entry 3025 (class 0 OID 0)
-- Dependencies: 210
-- Name: jobs_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public.jobs_id_seq', 1, false);


--
-- TOC entry 3026 (class 0 OID 0)
-- Dependencies: 209
-- Name: outputTypes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public."outputTypes_id_seq"', 1, false);


--
-- TOC entry 3027 (class 0 OID 0)
-- Dependencies: 208
-- Name: outputs_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public.outputs_id_seq', 1, false);


--
-- TOC entry 3028 (class 0 OID 0)
-- Dependencies: 207
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public.users_id_seq', 1, false);


--
-- TOC entry 2867 (class 2606 OID 18992)
-- Name: jobs jobs_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_pkey PRIMARY KEY (id);


--
-- TOC entry 2869 (class 2606 OID 18994)
-- Name: outputTypes outputTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public."outputTypes"
    ADD CONSTRAINT "outputTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2871 (class 2606 OID 18996)
-- Name: outputs outputs_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_pkey PRIMARY KEY (id);


--
-- TOC entry 2865 (class 2606 OID 18998)
-- Name: jobStatusTypes statusTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public."jobStatusTypes"
    ADD CONSTRAINT "statusTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2873 (class 2606 OID 19000)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 2874 (class 2606 OID 19001)
-- Name: jobs jobs_jobstatustypeid_fk; Type: FK CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_jobstatustypeid_fk FOREIGN KEY ("jobStatusTypeId") REFERENCES public."jobStatusTypes"(id) NOT VALID;


--
-- TOC entry 2875 (class 2606 OID 19006)
-- Name: jobs jobs_userid_fk; Type: FK CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_userid_fk FOREIGN KEY ("userId") REFERENCES public.users(id) ON DELETE CASCADE NOT VALID;


--
-- TOC entry 2876 (class 2606 OID 19011)
-- Name: outputs outputs_jobid_fk; Type: FK CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_jobid_fk FOREIGN KEY ("jobId") REFERENCES public.jobs(id) NOT VALID;


--
-- TOC entry 2877 (class 2606 OID 19016)
-- Name: outputs outputs_typeid_fk; Type: FK CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_typeid_fk FOREIGN KEY ("outputTypeId") REFERENCES public."outputTypes"(id) NOT VALID;


-- Completed on 2024-01-20 15:23:23 EST

--
-- PostgreSQL database dump complete
--

