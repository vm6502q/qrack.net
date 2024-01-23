--
-- PostgreSQL database dump
--

-- Dumped from database version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)
-- Dumped by pg_dump version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)

-- Started on 2024-01-20 16:37:34 EST

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
-- TOC entry 202 (class 1259 OID 20473)
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
-- TOC entry 203 (class 1259 OID 20479)
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
-- TOC entry 3024 (class 0 OID 0)
-- Dependencies: 203
-- Name: jobStatusTypes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public."jobStatusTypes_id_seq" OWNED BY public."jobStatusTypes".id;


--
-- TOC entry 204 (class 1259 OID 20481)
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
-- TOC entry 205 (class 1259 OID 20487)
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
-- TOC entry 3025 (class 0 OID 0)
-- Dependencies: 205
-- Name: jobs_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public.jobs_id_seq OWNED BY public.jobs.id;


--
-- TOC entry 206 (class 1259 OID 20489)
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
-- TOC entry 207 (class 1259 OID 20495)
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
-- TOC entry 3026 (class 0 OID 0)
-- Dependencies: 207
-- Name: outputTypes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public."outputTypes_id_seq" OWNED BY public."outputTypes".id;


--
-- TOC entry 208 (class 1259 OID 20497)
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
-- TOC entry 209 (class 1259 OID 20503)
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
-- TOC entry 3027 (class 0 OID 0)
-- Dependencies: 209
-- Name: outputs_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public.outputs_id_seq OWNED BY public.outputs.id;


--
-- TOC entry 210 (class 1259 OID 20505)
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
-- TOC entry 211 (class 1259 OID 20511)
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
-- TOC entry 3028 (class 0 OID 0)
-- Dependencies: 211
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: qracknet
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- TOC entry 2859 (class 2604 OID 20513)
-- Name: jobStatusTypes id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public."jobStatusTypes" ALTER COLUMN id SET DEFAULT nextval('public."jobStatusTypes_id_seq"'::regclass);


--
-- TOC entry 2860 (class 2604 OID 20514)
-- Name: jobs id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.jobs ALTER COLUMN id SET DEFAULT nextval('public.jobs_id_seq'::regclass);


--
-- TOC entry 2861 (class 2604 OID 20515)
-- Name: outputTypes id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public."outputTypes" ALTER COLUMN id SET DEFAULT nextval('public."outputTypes_id_seq"'::regclass);


--
-- TOC entry 2862 (class 2604 OID 20516)
-- Name: outputs id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.outputs ALTER COLUMN id SET DEFAULT nextval('public.outputs_id_seq'::regclass);


--
-- TOC entry 2863 (class 2604 OID 20517)
-- Name: users id; Type: DEFAULT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- TOC entry 3009 (class 0 OID 20473)
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
-- TOC entry 3011 (class 0 OID 20481)
-- Dependencies: 204
-- Data for Name: jobs; Type: TABLE DATA; Schema: public; Owner: qracknet
--

COPY public.jobs (id, "createdAt", "updatedAt", "jobStatusTypeId", "statusMessage", "userId") FROM stdin;
\.


--
-- TOC entry 3013 (class 0 OID 20489)
-- Dependencies: 206
-- Data for Name: outputTypes; Type: TABLE DATA; Schema: public; Owner: qracknet
--

COPY public."outputTypes" (id, "createdAt", "updatedAt", name) FROM stdin;
1	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	suid
2	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	bool
3	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	double
4	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	bitCapInt
5	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	std::vector<bitCapInt>
6	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	std::vector<double>
7	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	nuid
\.


--
-- TOC entry 3015 (class 0 OID 20497)
-- Dependencies: 208
-- Data for Name: outputs; Type: TABLE DATA; Schema: public; Owner: qracknet
--

COPY public.outputs (id, "createdAt", "updatedAt", "jobId", "outputTypeId", name, value) FROM stdin;
\.


--
-- TOC entry 3017 (class 0 OID 20505)
-- Dependencies: 210
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: qracknet
--

COPY public.users (id, "createdAt", "updatedAt", username, "usernameNormal", "passwordHash", affiliation, name, email, "recoveryToken", "recoveryTokenExpiration") FROM stdin;
\.


--
-- TOC entry 3029 (class 0 OID 0)
-- Dependencies: 203
-- Name: jobStatusTypes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public."jobStatusTypes_id_seq"', 1, false);


--
-- TOC entry 3030 (class 0 OID 0)
-- Dependencies: 205
-- Name: jobs_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public.jobs_id_seq', 1, false);


--
-- TOC entry 3031 (class 0 OID 0)
-- Dependencies: 207
-- Name: outputTypes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public."outputTypes_id_seq"', 1, false);


--
-- TOC entry 3032 (class 0 OID 0)
-- Dependencies: 209
-- Name: outputs_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public.outputs_id_seq', 1, false);


--
-- TOC entry 3033 (class 0 OID 0)
-- Dependencies: 211
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: qracknet
--

SELECT pg_catalog.setval('public.users_id_seq', 1, false);


--
-- TOC entry 2869 (class 2606 OID 20519)
-- Name: jobs jobs_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_pkey PRIMARY KEY (id);


--
-- TOC entry 2871 (class 2606 OID 20521)
-- Name: outputTypes outputTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public."outputTypes"
    ADD CONSTRAINT "outputTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2876 (class 2606 OID 20523)
-- Name: outputs outputs_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_pkey PRIMARY KEY (id);


--
-- TOC entry 2865 (class 2606 OID 20525)
-- Name: jobStatusTypes statusTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public."jobStatusTypes"
    ADD CONSTRAINT "statusTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2878 (class 2606 OID 20527)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 2866 (class 1259 OID 20533)
-- Name: fki_jobs_jobStatusTypeId_fkey; Type: INDEX; Schema: public; Owner: qracknet
--

CREATE INDEX "fki_jobs_jobStatusTypeId_fkey" ON public.jobs USING btree ("jobStatusTypeId");


--
-- TOC entry 2867 (class 1259 OID 20539)
-- Name: fki_jobs_userId_fkey; Type: INDEX; Schema: public; Owner: qracknet
--

CREATE INDEX "fki_jobs_userId_fkey" ON public.jobs USING btree ("userId");


--
-- TOC entry 2872 (class 1259 OID 20545)
-- Name: fki_outputs_jobId_fkey; Type: INDEX; Schema: public; Owner: qracknet
--

CREATE INDEX "fki_outputs_jobId_fkey" ON public.outputs USING btree ("jobId");


--
-- TOC entry 2873 (class 1259 OID 20546)
-- Name: fki_outputs_jobId_name_fkey; Type: INDEX; Schema: public; Owner: qracknet
--

CREATE INDEX "fki_outputs_jobId_name_fkey" ON public.outputs USING btree ("jobId", name);


--
-- TOC entry 2874 (class 1259 OID 20552)
-- Name: fki_outputs_outputTypeId_fkey; Type: INDEX; Schema: public; Owner: qracknet
--

CREATE INDEX "fki_outputs_outputTypeId_fkey" ON public.outputs USING btree ("outputTypeId");


--
-- TOC entry 2879 (class 2606 OID 20528)
-- Name: jobs jobs_jobStatusTypeId_fk; Type: FK CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT "jobs_jobStatusTypeId_fk" FOREIGN KEY ("jobStatusTypeId") REFERENCES public."jobStatusTypes"(id) NOT VALID;


--
-- TOC entry 2880 (class 2606 OID 20534)
-- Name: jobs jobs_userId_fk; Type: FK CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT "jobs_userId_fk" FOREIGN KEY ("userId") REFERENCES public.users(id) ON DELETE CASCADE NOT VALID;


--
-- TOC entry 2881 (class 2606 OID 20540)
-- Name: outputs outputs_jobId_fk; Type: FK CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT "outputs_jobId_fk" FOREIGN KEY ("jobId") REFERENCES public.jobs(id) NOT VALID;


--
-- TOC entry 2882 (class 2606 OID 20547)
-- Name: outputs outputs_outputTypeId_fk; Type: FK CONSTRAINT; Schema: public; Owner: qracknet
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT "outputs_outputTypeId_fk" FOREIGN KEY ("outputTypeId") REFERENCES public."outputTypes"(id) NOT VALID;


-- Completed on 2024-01-20 16:37:34 EST

--
-- PostgreSQL database dump complete
--

